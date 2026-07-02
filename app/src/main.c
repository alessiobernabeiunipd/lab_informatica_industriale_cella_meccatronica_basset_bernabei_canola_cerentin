#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "parsing.h"
#include "cella.h"
#include "controllore.h"

int main(void) {
    printf("=== AVVIO SIMULATORE CELLA MECCATRONICA ===\n\n");

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
        fprintf(stderr, "Errore: impossibile caricare i parametri di simulazione.\n");
        return 1;
    }

    catalogo_entry *catalogo = NULL;
    int num_cat = parse_catalogo("input/catalogo.csv", &catalogo);
    if (num_cat <= 0) {
        fprintf(stderr, "Errore: impossibile caricare il catalogo o catalogo vuoto.\n");
        return 1;
    }

    ordine_entry *ordini = NULL;
    int num_ordini = parse_ordini("input/ordini.csv", &ordini);
    if (num_ordini <= 0) {
        fprintf(stderr, "Errore: impossibile caricare gli ordini o nessun ordine presente.\n");
        free(catalogo);
        return 1;
    }

    printf("--- FASE 1 Completata: Configurazione Caricata ---\n");
    printf("  - Parametri di simulazione pronti.\n");
    printf("  - %d elementi presenti nel catalogo.\n", num_cat);
    printf("  - %d ordini di produzione caricati.\n\n", num_ordini);


    /*
     * FASE 2: INIZIALIZZAZIONE DELLA CELLA E DELLE STAZIONI
     * 
     * - Dichiarare e inizializzare la struttura principale della cella meccatronica.
     * - Collegare le stazioni (laminazione, pressa, GOM, AGV) alle rispettive strutture 
     *   all'interno della cella meccatronica.
     * - Configurare i parametri iniziali estratti nella Fase 1 (es. capacità dei buffer, 
     *   durata massima della simulazione, temperatura iniziale, ecc.).
     */
    cella_meccatronica *cella = init_cella();
    if (cella == NULL) {
        fprintf(stderr, "Errore: inizializzazione della cella fallita.\n");
        free(catalogo);
        free(ordini);
        return 1;
    }

    // Configura i parametri estratti
    cella->param = params;
    cella->gom->t_GOM = cella->param.temperatura_ambiente_iniziale;

    printf("--- FASE 2 Completata: Cella e Stazioni Inizializzate ---\n");
    printf("  - Stato delle stazioni impostato a IDLE.\n");
    printf("  - Temperatura iniziale GOM: %.2f °C\n\n", cella->gom->t_GOM);


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
    free(cella);

    printf("=== SIMULAZIONE COMPLETATA CON SUCCESSO ===\n");
    return 0;
}
