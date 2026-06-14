#include <stdio.h>
#include "controllore.h"
#include "utils.h"

// Funzione che controlla tick per tick la cella meccatronica
void controllore_tick(cella_meccatronica *cella) {
    printf("--- TICK %d ---\n", cella->tick_corrente);

    // 1. Laminazione
    printf("  - Laminazione: stato = %s", status_station_to_string(cella->laminazione.stato));
    if (cella->laminazione.stato == BUSY) {
        if (cella->laminazione.pezzo_in_lavorazione != NULL) {
            printf(" (Pezzo ID: %d)", cella->laminazione.pezzo_in_lavorazione->id_pezzo);
        }
        if (cella->laminazione.tick_lavorazione_rimasti > 0) {
            cella->laminazione.tick_lavorazione_rimasti--;
            printf(", tick rimanenti = %d (aggiornato)", cella->laminazione.tick_lavorazione_rimasti);
            if (cella->laminazione.tick_lavorazione_rimasti == 0) {
                cella->laminazione.stato = IDLE;
                printf(" -> Lavorazione completata! Stato impostato a IDLE.");
            }
        } else {
            printf(", tick rimanenti = %d", cella->laminazione.tick_lavorazione_rimasti);
        }
    } else if (cella->laminazione.stato == BROKEN) {
        printf(" (Lavorazione in pausa), tick rimanenti = %d", cella->laminazione.tick_lavorazione_rimasti);
    }
    printf("\n");

    // 2. Pressa
    printf("  - Pressa: stato = %s", status_station_to_string(cella->pressa.stato));
    if (cella->pressa.stato == BUSY) {
        if (cella->pressa.pezzo_in_lavorazione != NULL) {
            printf(" (Pezzo ID: %d)", cella->pressa.pezzo_in_lavorazione->id_pezzo);
        }
        if (cella->pressa.tick_lavorazione_rimasti > 0) {
            cella->pressa.tick_lavorazione_rimasti--;
            printf(", tick rimanenti = %d (aggiornato)", cella->pressa.tick_lavorazione_rimasti);
            if (cella->pressa.tick_lavorazione_rimasti == 0) {
                cella->pressa.stato = IDLE;
                printf(" -> Lavorazione completata! Stato impostato a IDLE.");
            }
        } else {
            printf(", tick rimanenti = %d", cella->pressa.tick_lavorazione_rimasti);
        }
    } else if (cella->pressa.stato == BROKEN) {
        printf(" (Lavorazione in pausa), tick rimanenti = %d", cella->pressa.tick_lavorazione_rimasti);
    }
    printf("\n");

    // 3. GOM
    printf("  - GOM: stato = %s", status_station_to_string(cella->gom.stato));
    if (cella->gom.stato == BUSY) {
        if (cella->gom.pezzo_in_lavorazione != NULL) {
            printf(" (Pezzo ID: %d)", cella->gom.pezzo_in_lavorazione->id_pezzo);
        }
        if (cella->gom.tick_lavorazione_rimasti > 0) {
            cella->gom.tick_lavorazione_rimasti--;
            printf(", tick rimanenti = %d (aggiornato)", cella->gom.tick_lavorazione_rimasti);
            if (cella->gom.tick_lavorazione_rimasti == 0) {
                cella->gom.stato = IDLE;
                printf(" -> Lavorazione completata! Stato impostato a IDLE.");
            }
        } else {
            printf(", tick rimanenti = %d", cella->gom.tick_lavorazione_rimasti);
        }
    } else if (cella->gom.stato == BROKEN) {
        printf(" (Lavorazione in pausa), tick rimanenti = %d", cella->gom.tick_lavorazione_rimasti);
    }
    printf("\n");

    // 4. AGV
    printf("  - AGV: stato = %s", status_station_to_string(cella->agv.stato));
    if (cella->agv.stato == BUSY) {
        if (cella->agv.pezzo_in_lavorazione != NULL) {
            printf(" (Pezzo ID: %d)", cella->agv.pezzo_in_lavorazione->id_pezzo);
        }
        if (cella->agv.tick_lavorazione_rimasti > 0) {
            cella->agv.tick_lavorazione_rimasti--;
            printf(", tick rimanenti = %d (aggiornato)", cella->agv.tick_lavorazione_rimasti);
            if (cella->agv.tick_lavorazione_rimasti == 0) {
                cella->agv.stato = IDLE;
                printf(" -> Lavorazione completata! Stato impostato a IDLE.");
            }
        } else {
            printf(", tick rimanenti = %d", cella->agv.tick_lavorazione_rimasti);
        }
    } else if (cella->agv.stato == BROKEN) {
        printf(" (Lavorazione in pausa), tick rimanenti = %d", cella->agv.tick_lavorazione_rimasti);
    }
    printf("\n");
}
