#include <stdio.h>
#include "types.h"
#include "controllore.h"

int main(void) {
    cella_meccatronica cella;
    cella.tick_corrente = 0;
    cella.tick_fine_simulazione = 10; // Impostiamo una simulazione di prova di 10 tick

    printf("=== AVVIO SIMULATORE CELLA MECCATRONICA ===\n");
    
    // Ciclo temporale discreto della simulazione
    while (cella.tick_corrente < cella.tick_fine_simulazione) {
        cella.tick_corrente++;
        controllore_tick(&cella);
    }

    printf("=== SIMULAZIONE COMPLETATA CON SUCCESSO ===\n");
    return 0;
}
