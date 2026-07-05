#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "controller.h"
#include "logger.h"
#include "parsing.h"
#include "utils.h"
#include "metrics.h"
#include "report.h"
#include "comparison.h"
#include "comparator.h"

// Esegue una singola simulazione completa con la politica contenuta in params.
// Alloca e libera internamente la cella e la lista dei pezzi di questa run.
// Ritorna una copia delle metriche finali.
static metriche_t esegui(parametri_simulazione params,
                         catalogo_entry *catalogo, int num_cat,
                         ordine_entry *ordini, int num_ordini, const char *report_path) {
    metriche_t risultato = {0};

    if(params.n_presse < 1){
        log_error_f(0, "Numero di presse inserito %d invalido, la simulazione verrà eseguita con 1 pressa", params.n_presse);
        params.n_presse = 1;
    }

    if(params.capacita_buffer_gom < 1){
        log_error_f(0, "Capacità del buffer gom inserita %d invalida", params.capacita_buffer_gom);
    }
    if(params.capacita_buffer_laminazione < 1){
        log_error_f(0, "Capacità del buffer laminazione inserita %d invalida", params.capacita_buffer_laminazione);
    } 
    if(params.capacita_buffer_pressa < 1){
        log_error_f(0, "Capacità del buffer pressa inserita %d invalida", params.capacita_buffer_pressa);
    }
    if(params.temperatura_ambiente_iniziale > GOM_TMAX){
        log_error_f(0, "Temperatura ambiente iniziale %d troppo elevata, GOM fuori uso", params.temperatura_ambiente_iniziale);
    }
    if(params.durata_simulazione_max < 0){
        log_error_f(0, "Durata della simulazione %d invalida", params.durata_simulazione_max);
        params.durata_simulazione_max = 0;
    }

    // Inizializzazione delle stazioni, dei buffer e della cella meccatronica
    // Nota: le metriche vengono inizializzate dal controller (metrics_init con il
    // conteggio effettivo dei pezzi), quindi qui NON va chiamata per evitare doppia init.
    cella_meccatronica *cella = init_cella(params);
    if (cella == NULL) {
        log_error(0, "Errore: inizializzazione della cella fallita.");
        return risultato;
    }

    printf("Cella e Stazioni Inizializzate\n");
    printf("  - Stato delle stazioni impostato a IDLE.\n");
    printf("  - Temperatura iniziale GOM: %.2f °C\n\n", cella->gom->t_GOM);

    // Generazione della lista dei pezzi dagli ordini attingendo ai valori nominali del catalogo
    int total_pieces = genera_pezzi_da_ordini(cella, ordini, num_ordini, catalogo, num_cat);
    if (total_pieces < 0) {
        // Libera i buffer e le stazioni in caso di fallimento per evitare leak
        terminate(cella->buf_lam);
        terminate(cella->buf_pressa);
        terminate(cella->buf_gom);
        if (cella->laminazione) free(cella->laminazione);
        if (cella->pressa) free_presse(cella->pressa, params.n_presse);
        if (cella->gom) free(cella->gom);
        if (cella->agv) free(cella->agv);
        free(cella);
        return risultato;
    }

    printf("Generazione Pezzi Svolta\n");
    printf("  - Generati in totale %d pezzi dagli ordini.\n\n", total_pieces);

    // Stampa a terminale la lista dei pezzi da produrre
    print_pezzi(cella->list_head);

    printf("Avvio del ciclo di simulazione...\n");

    // Esecuzione dell'intero ciclo della simulazione (loop temporale discreto)
    controller(cella);

    // Log di completamento del ciclo di simulazione
    log_info_f(cella->tick_corrente - 1, "Ciclo di simulazione terminato.");

    // Consolidamento finale e calcolo delle metriche a simulazione conclusa
    metrics_finalize();
    risultato = *metrics_get();   // copia per valore PRIMA di metrics_destroy

    // Stampa a video del report finale delle statistiche
    report_print(params.politica_controllo, &risultato);

    // Scrittura del report statistico finale su file di testo
    if (report_write(report_path, params.politica_controllo, &risultato) != 0) {
        log_error(cella->tick_corrente, "Errore nella scrittura del report su file.");
    }

    // Rilascio della memoria della cella e dei pezzi di questa run
    pezzo *p_curr = cella->list_head;
    while (p_curr != NULL) {
        pezzo *p_next = p_curr->next;
        free(p_curr);
        p_curr = p_next;
    }

    if (cella->laminazione) free(cella->laminazione);
    if (cella->pressa) free_presse(cella->pressa, params.n_presse);
    if (cella->gom) free(cella->gom);
    if (cella->agv) free(cella->agv);

    if (cella->buf_lam) terminate(cella->buf_lam);
    if (cella->buf_pressa) terminate(cella->buf_pressa);
    if (cella->buf_gom) terminate(cella->buf_gom);

    free(cella);
    metrics_destroy();

    return risultato;
}

int main(void) {
    printf("=== AVVIO SIMULATORE CELLA MECCATRONICA ===\n\n");

    // Caricamento dei parametri di simulazione da file CSV o TXT
    parametri_simulazione params;
    int param_res = parse_parametri("input/simulation_parameters.csv", &params);
    if (param_res != 0) {
        param_res = parse_parametri("input/simulation_parameters.txt", &params);
    }
    if (param_res != 0) {
        log_error(0, "Errore: impossibile caricare i parametri di simulazione.");
        logger_close();
        return 1;
    }

    // Seed base scelto una sola volta per esecuzione: condiviso da tutte e 4 le run (dati confrontabili),
    // ma diverso a ogni lancio del programma perche' derivato dall'orario corrente.
    params.seed = (unsigned int)time(NULL);

    parametri_simulazione params_a = params;
    strcpy(params_a.politica_controllo, "fcfs");
    parametri_simulazione params_b = params;
    strcpy(params_b.politica_controllo, "priorita");

    // Caricamento del catalogo dei pezzi dal file CSV
    catalogo_entry *catalogo = NULL;
    int num_cat = parse_catalogo("input/catalogo.csv", &catalogo);
    if (num_cat <= 0) {
        log_error(0, "Errore: impossibile caricare il catalogo o catalogo vuoto.");
        logger_close();
        return 1;
    }

    // Caricamento degli ordini di produzione dal file CSV
    ordine_entry *ordini = NULL;
    int num_ordini = parse_ordini("input/ordini.csv", &ordini);
    if (num_ordini <= 0) {
        log_error(0, "Errore: impossibile caricare gli ordini o nessun ordine presente.");
        free(catalogo);
        logger_close();
        return 1;
    }

    printf("Configurazione Caricata\n");
    printf("  - Parametri di simulazione pronti.\n");
    printf("  - %d elementi presenti nel catalogo.\n", num_cat);
    printf("  - %d ordini di produzione caricati.\n\n", num_ordini);

    // Run principali (con il numero di presse da configurazione), usate per il confronto tra politiche
    metriche_t m_a = esegui(params_a, catalogo, num_cat, ordini, num_ordini, "report_fcfs.txt");
    metriche_t m_b = esegui(params_b, catalogo, num_cat, ordini, num_ordini, "report_priorita.txt");

    // (1) Confronto tra politiche, a parità di presse: comparison_diff riempie c (0 se ok, -1 se errore)
    confronto_t c;
    comparison_diff(&m_a, "fcfs", &m_b, "priorita", &c);
    comparison_write("confronto.txt", &c);

    // Run a pressa singola, per confrontare il layout (1 pressa vs n_presse) a parità di politica
    parametri_simulazione params_a_1p = params_a;   // fcfs, una sola pressa
    params_a_1p.n_presse = 1;
    parametri_simulazione params_b_1p = params_b;   // priorita, una sola pressa
    params_b_1p.n_presse = 1;

    metriche_t m_a_1p = esegui(params_a_1p, catalogo, num_cat, ordini, num_ordini, "report_fcfs_1pressa.txt");
    metriche_t m_b_1p = esegui(params_b_1p, catalogo, num_cat, ordini, num_ordini, "report_priorita_1pressa.txt");

    // Etichette delle due configurazioni di layout messe a confronto (colonne della tabella)
    char nome_1p[32], nome_np[32];
    snprintf(nome_1p, sizeof(nome_1p), "1 pressa");
    snprintf(nome_np, sizeof(nome_np), "%d presse", params.n_presse);

    // (2) Confronto tra layout (1 pressa vs n_presse) per la politica fcfs
    confronto_t c_fcfs;
    comparison_diff(&m_a_1p, nome_1p, &m_a, nome_np, &c_fcfs);
    comparison_write("confronto_fcfs_presse.txt", &c_fcfs);

    // (3) Confronto tra layout (1 pressa vs n_presse) per la politica priorita
    confronto_t c_prio;
    comparison_diff(&m_b_1p, nome_1p, &m_b, nome_np, &c_prio);
    comparison_write("confronto_priorita_presse.txt", &c_prio);

    // Rilascio della memoria condivisa, caricata una sola volta
    free(catalogo);
    free(ordini);

    log_info(0, "=== SIMULAZIONE COMPLETATA CON SUCCESSO ===");
    logger_close();
    return 0;
}
