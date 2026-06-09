#include <stdio.h>
#include "controllore.h"

// Funzione che controlla tick per tick la cella meccatronica
void controllore_tick(cella_meccatronica *cella) {
    // Per ora effettua solamente la stampa del tick corrente a terminale
    printf("Tick corrente: %d\n", cella->tick_corrente);
}
