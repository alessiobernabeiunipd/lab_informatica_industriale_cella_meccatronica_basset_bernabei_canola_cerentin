#include <stdio.h>
#include <stdlib.h>

#include "controller.h"
#include "logger.h"

int main(void) {
    printf("=== AVVIO SIMULATORE CELLA MECCATRONICA ===\n\n");
    
    log_info(10, "ciao susino");
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


    /*
     * FASE 2: INIZIALIZZAZIONE DELLA CELLA E DELLE STAZIONI
     * 
     * - Dichiarare e inizializzare la struttura principale della cella meccatronica.
     * - Collegare le stazioni (laminazione, pressa, GOM, AGV) alle rispettive strutture 
     *   all'interno della cella meccatronica.
     * - Configurare i parametri iniziali estratti nella Fase 1 (es. capacità dei buffer, 
     *   durata massima della simulazione, temperatura iniziale, ecc.).
     */

    /*
     * FASE 3: GENERAZIONE DEI PEZZI DAGLI ORDINI
     * 
     * - Scorrere la lista degli ordini letti nella Fase 1.
     * - Per ciascun ordine, istanziare la quantità richiesta di pezzi.
     * - Per ogni pezzo creato, associare i valori nominali corrispondenti leggendoli dal catalogo 
     *   (tempo di laminazione, ciclo termico, deviazione max, area stampo, ecc.).
     * - Concatenare tutti i pezzi in una lista collegata globale (memorizzata nella cella).
     */


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


    /*
     * FASE 5: GENERAZIONE DEL REPORT FINALE E DELLE STATISTICHE
     * 
     * - Al termine del ciclo di simulazione, calcolare le statistiche finali sul lavoro svolto:
     *     - Numero totale di pezzi prodotti e di pezzi scartati.
     *     - Lead time medio dei pezzi completati.
     *     - Rispetto delle deadline degli ordini.
     * - Stampare a schermo o scrivere su file un report riassuntivo pulito e strutturato.
     */


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

    return 0;
}
