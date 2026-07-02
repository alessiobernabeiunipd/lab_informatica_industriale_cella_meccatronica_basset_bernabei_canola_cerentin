/*libreria per inizializzare e modificare la cella meccatronica*/
#ifndef CELLA_H
#define CELLA_H

#include "pezzo.h"

#define GOM_TMAX 27.0f
#define GOM_DEV_MIN 0.0f
#define GOM_DEV_MAX 2.0f
#define GOM_COOLING_TIME 10

//initialize cella with ticks and stats to zero, structs in idle state and head list to NULL
cella_meccatronica *init_cella();


//initialize agv
AGV *init_agv();

// Ritorna true se l'AGV è libero (IDLE), false se occupato (BUSY).
int agv_is_free(AGV *agv);

// Assegna un pezzo all'AGV e avvia il trasporto. tick_viaggio è la durata in tick.
void agv_get_mold(AGV *agv, pezzo *p, int tick_viaggio, int tick);

// Ritorna il pezzo se il viaggio è terminato e porta l'AGV in IDLE, altrimenti NULL.
pezzo *agv_unload(AGV *agv);

// Decrementa i tick rimanenti. Da chiamare ogni ciclo, è no-op se l'AGV è IDLE.
void agv_tick(AGV *agv);


//initialize pressa
stazione_pressa *init_pressa();

// Ritorna true se la stazione è libera (IDLE), false se occupata (BUSY).
int pressa_is_free(stazione_pressa *pressa);

// Carica un pezzo nella pressa e avvia il ciclo termico.
// Imposta i tick rimanenti dal tempo di cottura nominale del pezzo e aggiorna lo stato a IN_PRESS.
void pressa_load(stazione_pressa *pressa, pezzo *p, int tick);

// Ritorna il pezzo se il ciclo termico è terminato e porta la stazione in IDLE, altrimenti NULL.
// Da chiamare prima di pressa_tick().
pezzo *pressa_unload(stazione_pressa *pressa, int tick);

// Decrementa i tick rimanenti. Da chiamare ogni ciclo, è no-op se la stazione è IDLE.
void pressa_tick(stazione_pressa *pressa);


//initailize laminazione
stazione_laminazione *init_laminazione();

// Ritorna true se la stazione è libera (IDLE), false se occupata (BUSY).
int laminazione_is_free(stazione_laminazione *lam);

// Carica un pezzo nella stazione e avvia la laminazione.
// Imposta i tick rimanenti dal tempo nominale del pezzo e aggiorna lo stato a IN_LAMINATION.
void laminazione_load(stazione_laminazione *lam, pezzo *p, int tick);

// Ritorna il pezzo se la laminazione è terminata e porta la stazione in IDLE, altrimenti NULL.
// Da chiamare prima di laminazione_tick().
pezzo *laminazione_unload(stazione_laminazione *lam, int tick);

// Decrementa i tick rimanenti. Da chiamare ogni ciclo, è no-op se la stazione è IDLE.
void laminazione_tick(stazione_laminazione *lam);


//initialize gom
stazione_GOM *init_gom();

// Carica un pezzo nella stazione e avvia la scansione del GOM.
// Imposta i tick rimanenti dal tempo nominale del pezzo e aggiorna lo stato a IN_GOM.
void gom_load(stazione_GOM *gom, pezzo *p, int tick );

// Decrementa i tick rimanenti. Da chiamare ogni ciclo, è no-op se la stazione è IDLE.
// Se GOM è in COOLING, diminuisce i tick di cooling
// Se GOM è surriscaldato, lo mette in COOLING
// Alla fine del processo di cooling il GOM riprende un eventuale operazione interrotta.
void gom_tick(stazione_GOM *gom, parametri_simulazione param);

// Ritorna true se la stazione è libera (IDLE), false se occupata (BUSY).
int gom_is_free(stazione_GOM *gom);

// Ritorna il pezzo se la laminazione è terminata e porta la stazione in IDLE, altrimenti NULL.
// Da chiamare prima di laminazione_tick().
// Chiama gom_evaluate al suo interno
pezzo *gom_unload_and_evaluate(stazione_GOM *gom, int tick);

// Assegna randomicamente un valore di deviazione al pezzo e ritorna lo status corrispondente
// in base alle specifiche da catalogo
piece_status gom_evaluate(pezzo *p);

// Valuta lo stato di un pezzo:
// OK: aumenta contatore
// SCRAP: aumenta contatore e inserisce nella lista un nuovo pezzo
void quality_control ( int *ok, int *scrap, pezzo **list_head, pezzo *p);
#endif