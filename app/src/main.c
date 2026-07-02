#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "parsing.h"
#include "cella.h"
#include "controller.h"
#include "logger.h"

int main(void) {
    // Inizializza il logger
    if (logger_init("simulation.log") != 0) {
        fprintf(stderr, "Errore: impossibile inizializzare il logger.\n");
        return 1;
    }

    log_info(0, "=== AVVIO SIMULATORE CELLA MECCATRONICA ===");

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
    // TODO: implementare Fase 3


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
    // TODO: implementare Fase 4


    /*
     * FASE 5: GENERAZIONE DEL REPORT FINALE E DELLE STATISTICHE
     * 
     * - Al termine del ciclo di simulazione, calcolare le statistiche finali sul lavoro svolto:
     *     - Numero totale di pezzi prodotti e di pezzi scartati.
     *     - Lead time medio dei pezzi completati.
     *     - Rispetto delle deadline degli ordini.
     * - Stampare a schermo o scrivere su file un report riassuntivo pulito e strutturato.
     */
    // TODO: implementare Fase 5


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
