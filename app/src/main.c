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

    //inizializzazione del seed randomico
    srand(time(NULL));
    
      /*
     * FASE 1: PARSING E CARICAMENTO DELLE CONFIGURAZIONI
     * 
     * - Caricare i parametri di simulazione (es. da "input/simulation_parameters.csv" o ".txt").
     * - Caricare il catalogo dei pezzi (es. da "input/catalogo.csv").
     * - Caricare la lista degli ordini (es. da "input/ordini.csv").
     * - Verificare che tutti i caricamenti siano andati a buon fine; in caso di errore, 
     *   stampare un messaggio appropriato, liberare le risorse parziali ed uscire.
     * - Nota: Utilizzare esclusivamente le funzioni fornite dalla libreria di parsing per
     *   mantenere il main pulito.
     */
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

    catalogo_entry *catalogo = NULL;
    int num_cat = parse_catalogo("input/catalogo.csv", &catalogo);
    if (num_cat <= 0) {
        log_error(0, "Errore: impossibile caricare il catalogo o catalogo vuoto.");
        logger_close();
        return 1;
    }

    ordine_entry *ordini = NULL;
    int num_ordini = parse_ordini("input/ordini.csv", &ordini);
    if (num_ordini <= 0) {
        log_error(0, "Errore: impossibile caricare gli ordini o nessun ordine presente.");
        free(catalogo);
        logger_close();
        return 1;
    }

    log_info(0, "FASE 1 Completata: Configurazione Caricata");
    log_info(0, "  - Parametri di simulazione pronti.");
    log_info_f(0, "  - %d elementi presenti nel catalogo.", num_cat);
    log_info_f(0, "  - %d ordini di produzione caricati.", num_ordini);


    /*
     * FASE 2: INIZIALIZZAZIONE DELLA CELLA E DELLE STAZIONI
     * 
     * - Dichiarare e inizializzare la struttura principale della cella meccatronica.
     * - Collegare le stazioni (laminazione, pressa, GOM, AGV) alle rispettive strutture 
     *   all'interno della cella meccatronica.
     * - Configurare i parametri iniziali estratti nella Fase 1 (es. capacità dei buffer, 
     *   durata massima della simulazione, temperatura iniziale, ecc.).
     */
    metrics_init(num_ordini); // Inizializza le metriche con il numero di pezzi previsto dagli ordini
    cella_meccatronica *cella = init_cella(params);
    if (cella == NULL) {
        log_error(0, "Errore: inizializzazione della cella fallita.");
        free(catalogo);
        free(ordini);
        logger_close();
        return 1;
    }

    log_info(0, "FASE 2 Completata: Cella e Stazioni Inizializzate");
    log_info(0, "  - Stato delle stazioni impostato a IDLE.");
    log_info_f(0, "  - Temperatura iniziale GOM: %.2f °C", cella->gom->t_GOM);


    /*
     * FASE 3: GENERAZIONE DEI PEZZI DAGLI ORDINI
     * 
     * - Scorrere la lista degli ordini letti nella Fase 1.
     * - Per ciascun ordine, istanziare la quantità richiesta di pezzi.
     * - Per ogni pezzo creato, associare i valori nominali corrispondenti leggendoli dal catalogo 
     *   (tempo di laminazione, ciclo termico, deviazione max, area stampo, ecc.).
     * - Concatenare tutti i pezzi in una lista collegata globale (memorizzata nella cella).
     */
    int total_pieces = genera_pezzi_da_ordini(cella, ordini, num_ordini, catalogo, num_cat);
    if (total_pieces < 0) {
        free(catalogo);
        free(ordini);
        // Libera i buffer e la cella
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

    log_info(0, "FASE 3 Completata: Generazione Pezzi Svolta");
    log_info_f(0, "  - Generati in totale %d pezzi dagli ordini.", total_pieces);

    print_pezzi(cella->list_head);


    /*
     * FASE 4: AVVIO E ESECUZIONE DEL CICLO DELLA SIMULAZIONE (LOOP DEI TICK)
     * 
     * - Eseguire un ciclo temporale discreto (loop) che incrementa il tick corrente.
     * - Il ciclo continua finché il tick corrente è inferiore alla durata massima definita.
     * - All'interno di ogni tick:
     *     1. Chiamare la funzione principale del controllore (es. controllore_tick(&cella)) 
     *        che gestisce l'avanzamento dello stato delle macchine, i movimenti dell'AGV 
     *        e lo spostamento dei pezzi tra i buffer.
     *     2. (Opzionale) Stampare o loggare lo stato corrente della cella a fini di debug/simulazione.
     */
    log_info(0, "FASE 4: Avvio del ciclo di simulazione...");
    controller(cella);
    log_info_f(cella->tick_corrente - 1, "FASE 4 Completata: Ciclo di simulazione terminato.");


    /*
     * FASE 5: GENERAZIONE DEL REPORT FINALE E DELLE STATISTICHE
     *
     * - Al termine del ciclo di simulazione, calcolare le statistiche finali sul lavoro svolto:
     *     - Numero totale di pezzi prodotti e di pezzi scartati.
     *     - Lead time medio dei pezzi completati.
     *     - Rispetto delle deadline degli ordini.
     * - Stampare a schermo o scrivere su file un report riassuntivo pulito e strutturato.
     */
    metrics_finalize();
    const metriche_t *m_finali = metrics_get();

    // stampa a video il report
    report_print("FCFS", m_finali);

    // scrive il report su file
    if (report_write("report.txt", "FCFS", m_finali) != 0) {
        log_error(cella->tick_corrente, "Errore nella scrittura del report su file.");
    }
    //if (csv_write("report.csv", "FCFS", m_finali) != 0){
    //    log_error(cella->tick_corrente, "Errore nella scrittura del report CSV su file");
    //}

    /*
     * FASE 6: RILASCIO DELLE RISORSE (CLEANUP)
     * 
     * - Deallocare tutta la memoria allocata dinamicamente durante l'esecuzione del programma:
     *     - Lista dei pezzi generati.
     *     - Catalogo dei pezzi.
     *     - Lista degli ordini.
     *     - Eventuali altre strutture allocate dinamicamente.
     * - Terminare l'esecuzione restituendo 0 in caso di successo.
     */
    // Rilascio memoria config
    free(catalogo);
    free(ordini);

    // Rilascio memoria pezzi
    pezzo *p_curr = cella->list_head;
    while (p_curr != NULL) {
        pezzo *p_next = p_curr->next;
        free(p_curr);
        p_curr = p_next;
    }

    // Rilascio memoria cella e stazioni
    if (cella->laminazione) free(cella->laminazione);
    if (cella->pressa) free(cella->pressa);
    if (cella->gom) free(cella->gom);
    if (cella->agv) free(cella->agv);
    
    // Libera i buffer della cella
    if (cella->buf_lam) terminate(cella->buf_lam);
    if (cella->buf_pressa) terminate(cella->buf_pressa);
    if (cella->buf_gom) terminate(cella->buf_gom);
    
    free(cella);

    log_info(0, "=== SIMULAZIONE COMPLETATA CON SUCCESSO ===");
    logger_close();
    return 0;
}
