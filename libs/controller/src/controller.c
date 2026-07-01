#include "controller.h"

static void unload_all(cella_meccatronica *c){
    //scarico il pezzo dal gom e valuto la sua conformità alle specifiche
    pezzo *pgom = gom_unload_and_evaluate(c->gom, c->tick_corrente);
    if(pgom != NULL)
        quality_control(&(c->pezzi_completati), &(c->pezzi_scartati), c->list_head, pgom);
    
    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla pressa

    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla laminazione

    // valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dall'agv

}

static void load_all(cella_meccatronica *c){
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    
    // trovo il primo pezzo che non ha ancora iniziato lavorazione e dico al gom di prendere lo stampo
    pezzo *pagv = first_pezzo_with_status(c->list_head, CREATED);
    agv_get_mold(c->agv, pagv, c->tempi.magazzino_lam, c->tick_corrente);
} 

static void tick_all(cella_meccatronica *c){
   //decremento i tick di lavorazione gom
    gom_tick(c->gom, c->param); 
    //decremento i tick di lavorazione pressa
    pressa_tick(c->pressa);
    //decremento i tick di lavorazione laminazione
    laminazione_tick(c->laminazione);
    //decremento i tick di lavorazione agv
    agv_tick(c->agv);
}

void controller(cella_meccatronica *c){
    //chiamo le funzioni di controllo. La strategia adottata prevede tre fasi (load, unload, tick) per quattro stazioni.
    //vengono prima effettuati tutti gli unload, poi load, infine tick
    unload_all(c);
    load_all(c);
    tick_all(c);

    //incremento il conteggio dei tick che tengono traccia dello scorrere del tempo
    c->tick_corrente++;
}