/*libreria per inizializzare e modificare la cella meccatronica*/
#ifndef CELLA_H
#define CELLA_H

#include "pezzo.h"

//initialize cella with ticks and stats to zero, structs in idle state and head list to NULL
cella_meccatronica *init_cella();

//initialize agv
AGV *init_agv();

// Ritorna true se l'AGV è libero (IDLE), false se occupato (BUSY).
int agv_is_free(AGV *agv);

// Assegna un pezzo all'AGV e avvia il trasporto. tick_viaggio è la durata in tick.
void agv_preleva_stampo(AGV *agv, pezzo *p, int tick_viaggio);

// Avanza di un tick il trasporto. Da chiamare ogni ciclo, è no-op se l'AGV è IDLE.
void agv_tick(AGV *agv);

// Ritorna il pezzo se il viaggio è terminato e porta l'AGV in IDLE, altrimenti NULL.
// Da chiamare dopo agv_tick().
pezzo *agv_is_done(AGV *agv);



//initialize pressa
stazione_pressa *init_pressa();

//initailize laminazione
stazione_laminazione *init_laminazione();

//initialize gom
stazione_GOM *init_GOM();


#endif