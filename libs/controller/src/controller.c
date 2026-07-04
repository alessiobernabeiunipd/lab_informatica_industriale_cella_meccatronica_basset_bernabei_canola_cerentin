#include <stdio.h>
#include "controller.h"
#include "logger.h"
#include "metrics.h"


static void unload_all(cella_meccatronica *c){
    //scarico il pezzo dal gom e valuto la sua conformità alle specifiche
    pezzo *pgom = gom_unload_and_evaluate(c->gom, c->tick_corrente);
    if(pgom != NULL){
        quality_control(&(c->metrics.pezzi_completati), &(c->metrics.pezzi_scartati), &(c->list_head), pgom);
        if(pgom->stato == OK){
            record_piece_done(pgom);
            log_info_f(c->tick_corrente, "Pezzo %d scaricato dal GOM e completato (lead time %d tick)", pgom->id_pezzo, pgom->lead_time);
        } else {
            record_piece_scrap(pgom);
            log_warning_f(c->tick_corrente, "Pezzo %d scartato al GOM, reinserito in produzione", pgom->id_pezzo);
        }
    }

    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla pressa
    for (int i = 0; i < c->param.n_presse; i++) {

        if (is_full(c->buf_gom) == 0) {
            pezzo *ppres = pressa_unload(c->pressa[i], c->tick_corrente);

            if (ppres != NULL) {
                // log_info deve essere fatto dentro if sennò molte volte prenderebbe NULL e quindi crash
                log_info_f(c->tick_corrente, "Pezzo %d scaricato dalla pressa %d", ppres->id_pezzo, i + 1 );
                new_item(ppres, c->buf_gom);
                log_info_f(c->tick_corrente, "Pezzo %d inserito nel buffer GOM", ppres->id_pezzo);
                //non posso caricare due pezzi nello stesso tick per limitazioni dei buffer
                break; 
            }
        } 
        else {
            record_block();
            log_warning_f(c->tick_corrente, "Buffer GOM pieno: scarico dalla pressa %d bloccato", i);
            break; // se il buffer è pieno, inutile controllare le altre presse in questo tick
        }
    }
    //valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dalla laminazione
    if(is_full(c->buf_pressa)==0){
        pezzo *plam = laminazione_unload(c->laminazione, c->tick_corrente);
        if(plam != NULL){
            // log_info deve essere fatto dentro if sennò molte volte prenderebbe NULL e quindi crash
            log_info_f(c->tick_corrente, "Pezzo %d scaricato dalla laminazione", plam->id_pezzo);
            new_item(plam, c->buf_pressa);
            log_info_f(c->tick_corrente, "Pezzo %d inserito nel buffer pressa", plam->id_pezzo);
        }
    } else {
        record_block();
        log_warning_f(c->tick_corrente, "Buffer pressa pieno: scarico dalla laminazione bloccato");
    }
    // valuto se il buffer può ricevere un altro pezzo, poi scarico il pezzo dall'agv
    if(is_full(c->buf_lam)==0){
        pezzo *pagv = agv_unload(c->agv);
        if(pagv != NULL){
            // log_info deve essere fatto dentro if sennò molte volte prenderebbe NULL e quindi crash
            log_info_f(c->tick_corrente, "Pezzo %d scaricato dall'AGV", pagv->id_pezzo);
            new_item(pagv, c->buf_lam);
            log_info_f(c->tick_corrente, "Pezzo %d inserito nel buffer laminazione", pagv->id_pezzo);
        }
    } else {
        record_block();
        log_warning_f(c->tick_corrente, "Buffer laminazione pieno: scarico dall'AGV bloccato");
    }
}

static void load_all(cella_meccatronica *c){
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(gom_is_free(c->gom) && !is_empty(c->buf_gom)){
        pezzo *pgom = take_item(c->buf_gom);
        if(pgom != NULL){
            gom_load(c->gom, pgom, c->tick_corrente);
            log_info_f(c->tick_corrente, "Pezzo %d caricato sul GOM", pgom->id_pezzo);
        }
    }
    for (int i = 0; i < c->param.n_presse; i++) {
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
        if(pressa_is_free(c->pressa[i]) && !is_empty(c->buf_pressa)){
            pezzo *ppres = take_item(c->buf_pressa);
            if(ppres != NULL) {
                pressa_load(c->pressa[i], ppres, c->tick_corrente);
                log_info_f(c->tick_corrente, "Pezzo %d caricato sulla pressa %d", ppres->id_pezzo, i + 1);
            }
            
        }
    }
    // prendo dal buffer il primo elemento (puntatore a pezzo) e valuto se esiste o è NULL
    if(laminazione_is_free(c->laminazione) && !is_empty(c->buf_lam)){
        pezzo *plam = take_item(c->buf_lam);
        if(plam != NULL){
            laminazione_load(c->laminazione, plam, c->tick_corrente);
            log_info_f(c->tick_corrente, "Pezzo %d caricato sulla laminazione", plam->id_pezzo);
        }
    }
     
    //uso un case switch per selezionare politica di ctrl
    pezzo *pagv = NULL;
    if(strcmp(c->param.politica_controllo, "fcfs") == 0){
        // trovo il primo pezzo che non ha ancora iniziato lavorazione e dico al agv di prendere lo stampo
        pagv = first_pezzo_with_status(c->list_head, CREATED);
    }
    else if(strcmp(c->param.politica_controllo,"priorita") == 0){
        // trovo il primo pezzo tra quelli con  priorità maggiore che non ha ancora iniziato lavorazione e dico al agv di prendere lo stampo
        pagv = high_priority(c->list_head, CREATED);
    }
    else {
        log_error_f(c->tick_corrente, "Errore: Politica di controllo '%s' non valida. Deve essere 'fcfs' o 'priorita'.", c->param.politica_controllo);
    }
    if(pagv != NULL && agv_is_free(c->agv)){
            agv_get_mold(c->agv, pagv, c->param.tempo_agv, c->tick_corrente);
            log_info_f(c->tick_corrente, "AGV incaricato per il trasporto del pezzo %d", pagv->id_pezzo);
        }
    //log_info_f(c->tick_corrente, "Politica di controllo con priorità: pezzo %d caricato sull'AGV per trasporto", pagv->id_pezzo);
    
} 
// per mettere i messaggi di log solo nel controllore e non nelle singole macchine, vado a fare un controllo
// sullo stato del GOM prima di gom tick
static void tick_all(cella_meccatronica *c){
    station_status gom_stato_prima = c->gom->stato;
   //decremento i tick di lavorazione gom
    gom_tick(c->gom, c->param); 
    if (gom_stato_prima != COOLING && c->gom->stato == COOLING){
        log_warning_f(c->tick_corrente, "GOM in stato di raffreddamento: pezzo %d in lavorazione", c->gom->pezzo_in_lavorazione->id_pezzo);
    }
    //decremento i tick di lavorazione pressa
    for (int i = 0; i < c->param.n_presse; i++) 
        pressa_tick(c->pressa[i]);
    //decremento i tick di lavorazione laminazione
    laminazione_tick(c->laminazione);
    //decremento i tick di lavorazione agv
    agv_tick(c->agv);
}

void controller(cella_meccatronica *c){

    int total_pieces = 0;
    pezzo *curr = c->list_head;
    while (curr != NULL) {
        total_pieces++;
        curr = curr->next;
    }
    metrics_init(total_pieces);

    log_info_f(c->tick_corrente, "Avvio simulazione con politica %s: durata massima %d tick", c->param.politica_controllo , c->param.durata_simulazione_max);

    //inizializzo tempo per agv
    c->param.tempo_agv = TEMPO_AGV;

    while(c->tick_corrente <= c->param.durata_simulazione_max){
        //chiamo le funzioni di controllo. La strategia adottata prevede tre fasi (load, unload, tick) per quattro stazioni.
        //vengono prima effettuati tutti gli unload, poi load, infine tick
        unload_all(c);
        load_all(c);
        tick_all(c);

        //incremento il conteggio dei tick che tengono traccia dello scorrere del tempo
        c->tick_corrente++;
    }

    metrics_finalize();

    log_info_f(c->tick_corrente, "Simulazione terminata: %d pezzi completati, %d pezzi scartati",
               c->metrics.pezzi_completati, c->metrics.pezzi_scartati);
}