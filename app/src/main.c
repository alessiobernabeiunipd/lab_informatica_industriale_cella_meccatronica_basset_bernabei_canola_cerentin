#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "controller.h"
#include "logger.h"
#include "parsing.h"
#include "utils.h"
#include "metrics.h"
#include "report.h"

int main(void) {
    printf("=== AVVIO SIMULATORE CELLA MECCATRONICA ===\n\n");

    // Inizializzazione del seed per la generazione di numeri casuali
    srand(time(NULL));
    
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

    // Inizializzazione delle stazioni, dei buffer e della cella meccatronica
    // Nota: le metriche vengono inizializzate dal controller (metrics_init con il
    // conteggio effettivo dei pezzi), quindi qui NON va chiamata per evitare doppia init.
    cella_meccatronica *cella = init_cella(params);
    if (cella == NULL) {
        log_error(0, "Errore: inizializzazione della cella fallita.");
        free(catalogo);
        free(ordini);
        logger_close();
        return 1;
    }

    printf("Cella e Stazioni Inizializzate\n");
    printf("  - Stato delle stazioni impostato a IDLE.\n");
    printf("  - Temperatura iniziale GOM: %.2f °C\n\n", cella->gom->t_GOM);

    // Generazione della lista dei pezzi dagli ordini attingendo ai valori nominali del catalogo
    int total_pieces = genera_pezzi_da_ordini(cella, ordini, num_ordini, catalogo, num_cat);
    if (total_pieces < 0) {
        free(catalogo);
        free(ordini);
        // Libera i buffer e le stazioni in caso di fallimento per evitare leak
        terminate(cella->buf_lam);
        terminate(cella->buf_pressa);
        terminate(cella->buf_gom);
        if (cella->laminazione) free(cella->laminazione);
        if (cella->pressa) free(cella->pressa);
        if (cella->gom) free(cella->gom);
        if (cella->agv) free(cella->agv);
        free(cella);
        logger_close();
        return 1;
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
    const metriche_t *m_finali = metrics_get();

    // Stampa a video del report finale delle statistiche
    report_print("FCFS", m_finali);

    // Scrittura del report statistico finale su file di testo
    if (report_write("report.txt", "FCFS", m_finali) != 0) {
        log_error(cella->tick_corrente, "Errore nella scrittura del report su file.");
    }

    // Rilascio di tutta la memoria allocata dinamicamente
    free(catalogo);
    free(ordini);

    pezzo *p_curr = cella->list_head;
    while (p_curr != NULL) {
        pezzo *p_next = p_curr->next;
        free(p_curr);
        p_curr = p_next;
    }

    if (cella->laminazione) free(cella->laminazione);
    if (cella->pressa) free(cella->pressa);
    if (cella->gom) free(cella->gom);
    if (cella->agv) free(cella->agv);
    
    if (cella->buf_lam) terminate(cella->buf_lam);
    if (cella->buf_pressa) terminate(cella->buf_pressa);
    if (cella->buf_gom) terminate(cella->buf_gom);
    
    free(cella);
    metrics_destroy();

    // Chiusura del file di log
    log_info(0, "=== SIMULAZIONE COMPLETATA CON SUCCESSO ===");
    logger_close();
    return 0;
}
