#include "controller.h"
#include "logger.h"

static void unload_all(cella_meccatronica *c){
    //scarico il pezzo dal gom e valuto la sua conformità alle specifiche
    pezzo *pgom = gom_unload_and_evaluate(c->gom, c->tick_corrente);
    if(pgom != NULL) {
        if(pgom->stato == OK) {
            log_info_f(c->tick_corrente, "Stazione GOM: pezzo %d scaricato - esito VALUTAZIONE: CONFORME (deviazione: %.3f)", 
                       pgom->id_pezzo, pgom->deviazione_gom);
        } else {
            log_warning_f(c->tick_corrente, "Stazione GOM: pezzo %d scaricato - esito VALUTAZIONE: SCARTO (deviazione: %.3f)", 
                        pgom->id_pezzo, pgom->deviazione_gom);
            log_info_f(c->tick_corrente, "Controllo Qualità: Pezzo %d scartato, reinserito un nuovo pezzo in produzione per l'ordine %d", 
                       pgom->id_pezzo, pgom->ID_ordine);
        }
        quality_control(&(c->pezzi_completati), &(c->pezzi_scartati), (&c->list_head), pgom);
    }

    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla pressa
    if(is_full(c->buf_gom)==0){
        pezzo *ppres = pressa_unload(c->pressa, c->tick_corrente);
        if(ppres != NULL) {
            log_info_f(c->tick_corrente, "Stazione PRESSA: pezzo %d scaricato e inserito nel buffer GOM", ppres->id_pezzo);
            new_item(ppres, c->buf_gom);
        }
    }
    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla laminazione
    if(is_full(c->buf_pressa)==0){
        pezzo *plam = laminazione_unload(c->laminazione, c->tick_corrente);
        if(plam != NULL) {
            log_info_f(c->tick_corrente, "Stazione LAMINAZIONE: pezzo %d scaricato e inserito nel buffer pressa", plam->id_pezzo);
            new_item(plam, c->buf_pressa);
        }
    }
    // valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dall'agv
    if(is_full(c->buf_lam)==0){
        pezzo *pagv = agv_unload(c->agv);
        if(pagv != NULL) {
            log_info_f(c->tick_corrente, "Stazione AGV: pezzo %d scaricato e inserito nel buffer laminazione", pagv->id_pezzo);
            new_item(pagv, c->buf_lam);
        }
    }
} 

static void load_all(cella_meccatronica *c){
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(gom_is_free(c->gom) && !is_empty(c->buf_gom)){
        pezzo *pgom = take_item(c->buf_gom);
        if(pgom != NULL) {
            log_info_f(c->tick_corrente, "Stazione GOM: caricato pezzo %d da buffer GOM", pgom->id_pezzo);
            gom_load(c->gom, pgom, c->tick_corrente);
        }
    }
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(pressa_is_free(c->pressa) && !is_empty(c->buf_pressa)){
        pezzo *ppres = take_item(c->buf_pressa);
        if(ppres != NULL) {
            log_info_f(c->tick_corrente, "Stazione PRESSA: caricato pezzo %d da buffer pressa", ppres->id_pezzo);
            pressa_load(c->pressa, ppres, c->tick_corrente);
        }
    }
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(laminazione_is_free(c->laminazione) && !is_empty(c->buf_lam)){
        pezzo *plam = take_item(c->buf_lam);
        if(plam != NULL) {
            log_info_f(c->tick_corrente, "Stazione LAMINAZIONE: caricato pezzo %d da buffer laminazione", plam->id_pezzo);
            laminazione_load(c->laminazione, plam, c->tick_corrente);
        }
    }
     
    //uso un case switch per selezionare politica di ctrl
    if(strcmp(c->param.politica_controllo, "fcfs")==0){
        // trovo il primo pezzo che non ha ancora iniziato lavorazione e dico al gom di prendere lo stampo
        pezzo *pagv = first_pezzo_with_status(c->list_head, CREATED);
        if(pagv != NULL && agv_is_free(c->agv)) {
            log_info_f(c->tick_corrente, "Stazione AGV: prelievo pezzo %d (ordine %d, priorita %d) per trasporto", 
                       pagv->id_pezzo, pagv->ID_ordine, pagv->priorità);
            agv_get_mold(c->agv, pagv, c->param.tempo_agv, c->tick_corrente);
        }
    }
    //else if(strcmp(c->param.politica_controllo,"priorità"))
    
} 

static void tick_all(cella_meccatronica *c){
    // Salvo lo stato precedente del GOM per verificare transizioni a COOLING
    int was_busy = (c->gom->stato == BUSY);
    int was_cooling = (c->gom->stato == COOLING);

    //decremento i tick di lavorazione gom
    gom_tick(c->gom, c->param); 
    //decremento i tick di lavorazione pressa
    pressa_tick(c->pressa);
    //decremento i tick di lavorazione laminazione
    laminazione_tick(c->laminazione);
    //decremento i tick di lavorazione agv
    agv_tick(c->agv);

    // Log delle transizioni del GOM
    if (was_busy && c->gom->stato == COOLING) {
        log_warning_f(c->tick_corrente, "Stazione GOM: Temperatura critica raggiunta (%.2f °C). Avvio ciclo di raffreddamento (COOLING).", c->gom->t_GOM);
    } else if (was_cooling && c->gom->stato != COOLING) {
        log_info_f(c->tick_corrente, "Stazione GOM: Ciclo di raffreddamento completato. Temperatura resettata a %.2f °C.", c->gom->t_GOM);
    }
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