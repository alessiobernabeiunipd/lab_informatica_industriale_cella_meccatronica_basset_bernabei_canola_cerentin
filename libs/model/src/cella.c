#include <stdlib.h>
#include "../include/cella.h"

cella_meccatronica *init_cella(){
    cella_meccatronica *c = malloc(sizeof(cella_meccatronica));
    if (c == NULL) return NULL;
    
    c->laminazione = init_laminazione();
    if (c->laminazione == NULL) {
        free(c); 
        return NULL;
    }
    c->pressa = init_pressa();
    if (c->pressa == NULL) {
        free(c->laminazione);
        free(c); 
        return NULL;
    }
    c->gom = init_GOM();
    if (c->gom == NULL) {
        free(c->laminazione); 
        free(c->pressa); 
        free(c); 
        return NULL;
    }
    c->agv = init_agv();
    if (c->agv == NULL) {
        free(c->laminazione); 
        free(c->pressa); 
        free(c->gom); 
        free(c); 
        return NULL;
    }

    c->tempi.lam_pressa = 0;
    c->tempi.magazzino_lam = 0;
    c->tempi.magazzino_pressa = 0;

    c->tick_corrente = 0;
    c->tick_fine_simulazione = 0;
    c->pezzi_completati = 0;
    c->pezzi_scartati = 0;

    c->list_head = NULL;
    return c;
}


//AGV
AGV *init_agv(){
    AGV *agv = malloc(sizeof(AGV));
    if (agv == NULL) return NULL;
    agv->stato = IDLE;
    agv->tick_lavorazione_rimasti = 0;
    agv->pezzo_in_lavorazione = NULL;
    return agv;
}

int agv_is_free(AGV *agv) {
    return agv->stato == IDLE;
}

void agv_preleva_stampo(AGV *agv, pezzo *p, int tick_viaggio) {
    agv->stato = BUSY;
    agv->pezzo_in_lavorazione = p;
    agv->tick_lavorazione_rimasti = tick_viaggio;
}

void agv_tick(AGV *agv) {
    if (agv->stato == IDLE) return;
    agv->tick_lavorazione_rimasti--;
}

// Ritorna il pezzo quando arrivato, NULL altrimenti
pezzo *agv_is_done(AGV *agv) {
    if (agv->stato == BUSY && agv->tick_lavorazione_rimasti <= 0) {
        agv->stato = IDLE;
        pezzo *p = agv->pezzo_in_lavorazione;
        agv->pezzo_in_lavorazione = NULL;
        return p;
    }
    return NULL;
}



stazione_pressa *init_pressa(){
    stazione_pressa *pressa = malloc(sizeof(stazione_pressa));
    if (pressa == NULL) return NULL;
    pressa->stato = IDLE;
    pressa->tick_lavorazione_rimasti = 0;
    pressa->pezzo_in_lavorazione = NULL;
    return pressa;
}

stazione_laminazione *init_laminazione(){
    stazione_laminazione *lam = malloc(sizeof(stazione_laminazione));
    if (lam == NULL) return NULL;
    lam->stato = IDLE;
    lam->tick_lavorazione_rimasti = 0;
    lam->pezzo_in_lavorazione = NULL;
    return lam;
}

stazione_GOM *init_GOM(){
    stazione_GOM *gom = malloc(sizeof(stazione_GOM));
    if (gom == NULL) return NULL;
    gom->stato = IDLE;
    gom->tick_lavorazione_rimasti = 0;
    gom->pezzo_in_lavorazione = NULL;
    gom->temperatura_GOM = 0;
    return gom;
}