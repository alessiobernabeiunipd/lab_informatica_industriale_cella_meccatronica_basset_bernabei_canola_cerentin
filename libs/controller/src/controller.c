#include "controller.h"

static void unload_all(cella_meccatronica *c){
    //scarico il pezzo dal gom e valuto la sua conformità alle specifiche
    pezzo *pgom = gom_unload_and_evaluate(c->gom, c->tick_corrente);
    if(pgom != NULL)
        quality_control(&(c->pezzi_completati), &(c->pezzi_scartati), (&c->list_head), pgom);

    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla pressa
    if(is_full(c->buf_gom)==0){
        pezzo *ppres = pressa_unload(c->pressa, c->tick_corrente);
        if(ppres != NULL)
        new_item(ppres, c->buf_gom);
    }
    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla laminazione
    if(is_full(c->buf_pressa)==0){
        pezzo *plam = laminazione_unload(c->laminazione, c->tick_corrente);
        if(plam != NULL)
        new_item(plam, c->buf_pressa);
    }
    // valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dall'agv
    if(is_full(c->buf_lam)==0){
        pezzo *pagv = agv_unload(c->agv);
        if(pagv != NULL)
        new_item(pagv, c->buf_lam);
    }
} 

static void load_all(cella_meccatronica *c){
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(gom_is_free(c->gom) && !is_empty(c->buf_gom)){
        pezzo *pgom = take_item(c->buf_gom);
        if(pgom != NULL)
        gom_load(c->gom, pgom, c->tick_corrente);
    }
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(pressa_is_free(c->pressa) && !is_empty(c->buf_pressa)){
        pezzo *ppres = take_item(c->buf_pressa);
        if(ppres != NULL)
        pressa_load(c->pressa, ppres, c->tick_corrente);
    }
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(laminazione_is_free(c->laminazione) && !is_empty(c->buf_lam)){
        pezzo *plam = take_item(c->buf_lam);
        if(plam != NULL)
        laminazione_load(c->laminazione, plam, c->tick_corrente);
    }
     
    //uso un case switch per selezionare politica di ctrl
    if(strcmp(c->param.politica_controllo, "fcfs") == 0){
        // trovo il primo pezzo che non ha ancora iniziato lavorazione e dico al gom di prendere lo stampo
        pezzo *pagv = first_pezzo_with_status(c->list_head, CREATED);
        if(pagv != NULL)
        //non serve una condizione di controllo se agv è libero, è integrata in agv_get_mold.
        //agv non parte se non è libero
        agv_get_mold(c->agv, pagv, c->param.tempo_agv, c->tick_corrente);
    }
    //else if(strcmp(c->param.politica_controllo,"priorità"))
    
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
    while(c->tick_corrente <= c->param.durata_simulazione_max){
    //chiamo le funzioni di controllo. La strategia adottata prevede tre fasi (load, unload, tick) per quattro stazioni.
    //vengono prima effettuati tutti gli unload, poi load, infine tick
    unload_all(c);
    load_all(c);
    tick_all(c);

    //incremento il conteggio dei tick che tengono traccia dello scorrere del tempo
    c->tick_corrente++;
    }
}