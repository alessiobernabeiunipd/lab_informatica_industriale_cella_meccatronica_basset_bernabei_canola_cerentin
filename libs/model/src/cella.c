#include "cella.h"

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

void agv_get_mold(AGV *agv, pezzo *p, int tick_viaggio, int tick) {
    if(agv_is_free(agv)!=1) return;
    agv->stato = BUSY;
    agv->pezzo_in_lavorazione = p;
    agv->tick_lavorazione_rimasti = 2*tick_viaggio;
    p->stato = TRAVELING;

    p->ts.ingresso=tick;
}

void agv_tick(AGV *agv) {
    if (agv->stato == IDLE) return;
    agv->tick_lavorazione_rimasti--;
}

// Returns pezzo if arrived, NULL if not
pezzo *agv_unload(AGV *agv) {
    if (agv->stato == BUSY && agv->tick_lavorazione_rimasti <= 0) {
        agv->stato = IDLE;
        pezzo *p = agv->pezzo_in_lavorazione;
        agv->pezzo_in_lavorazione = NULL;
        p->stato = WAITING_INPUT;
        return p;
    }
    return NULL;
}


//PRESSA
stazione_pressa *init_pressa(){
    stazione_pressa *pressa = malloc(sizeof(stazione_pressa));
    if (pressa == NULL) return NULL;
    pressa->stato = IDLE;
    pressa->tick_lavorazione_rimasti = 0;
    pressa->pezzo_in_lavorazione = NULL;
    return pressa;
}

int pressa_is_free(stazione_pressa *pressa) {
    return pressa->stato == IDLE;
}

void pressa_load(stazione_pressa *pressa, pezzo *p, int tick) {
    if(pressa_is_free(pressa)!=1) return;
    pressa->stato = BUSY;
    pressa->pezzo_in_lavorazione = p;
    pressa->tick_lavorazione_rimasti = p->valori_nom.t_pressa_nominale;
    p->stato = IN_PRESS;

    p->ts.inizio_pressa = tick;
}

void pressa_tick(stazione_pressa *pressa) {
    if (pressa->stato == IDLE) return;
    pressa->tick_lavorazione_rimasti--;
}

pezzo *pressa_unload(stazione_pressa *pressa, int tick) {
    if (pressa->stato == BUSY && pressa->tick_lavorazione_rimasti <= 0) {
        pressa->stato = IDLE;
        pezzo *p = pressa->pezzo_in_lavorazione;
        pressa->pezzo_in_lavorazione = NULL;
        p->stato = ON_CONVEYOR;
        p->tempo_pressa_effettivo = tick - p->ts.inizio_pressa;
        return p;
    }
    return NULL;
}


//LAMINAZIONE
stazione_laminazione *init_laminazione(){
    stazione_laminazione *lam = malloc(sizeof(stazione_laminazione));
    if (lam == NULL) return NULL;
    lam->stato = IDLE;
    lam->tick_lavorazione_rimasti = 0;
    lam->pezzo_in_lavorazione = NULL;
    return lam;
}

int laminazione_is_free(stazione_laminazione *lam) {
    return lam->stato == IDLE;
}

void laminazione_load(stazione_laminazione *lam, pezzo *p, int tick) {
    if(laminazione_is_free(lam)!=1) return;
    lam->stato = BUSY;
    lam->pezzo_in_lavorazione = p;
    lam->tick_lavorazione_rimasti = p->valori_nom.t_laminazione_nominale;
    p->stato = IN_LAMINATION;
    p->ts.inizio_laminazione = tick;
}

void laminazione_tick(stazione_laminazione *lam) {
    if (lam->stato == IDLE) return;
    lam->tick_lavorazione_rimasti--;
}

// Returns pezzo if finished, NULL if not
pezzo *laminazione_unload(stazione_laminazione *lam, int tick) {
    if (lam->stato == BUSY && lam->tick_lavorazione_rimasti <= 0) {
        lam->stato = IDLE;
        pezzo *p = lam->pezzo_in_lavorazione;
        lam->pezzo_in_lavorazione = NULL;
        p->stato = WAITING_PRESS;
        p->tempo_laminazione_effettivo = tick - p->ts.inizio_laminazione;
        return p;
    }
    return NULL;
}


//GOM
stazione_GOM *init_GOM(){
    stazione_GOM *gom = malloc(sizeof(stazione_GOM));
    if (gom == NULL) return NULL;
    gom->stato = IDLE;
    gom->tick_lavorazione_rimasti = 0;
    gom->tick_cooling_rimasti = 0;
    gom->pezzo_in_lavorazione = NULL;
    gom->t_GOM = 0;
    gom->t_amb = 0;
    return gom;
}

int gom_is_free(stazione_GOM *gom){
    return gom->stato == IDLE;
}

void gom_load(stazione_GOM *gom, pezzo *p, int tick) {
    if(gom_is_free(gom)!=1) return;
    gom->stato = BUSY;
    gom->pezzo_in_lavorazione = p;
    gom->tick_lavorazione_rimasti = p->valori_nom.t_gom;
    p->stato = IN_GOM;
    p->ts.inizio_gom = tick;
}

void gom_tick(stazione_GOM *gom, parametri_simulazione param) {
    if (gom->stato == IDLE) return;
    if (gom->stato == COOLING) {
        gom->tick_cooling_rimasti--;
        return;
    }
    gom->tick_lavorazione_rimasti--;
    gom->t_GOM += param.temperatura_incremento_minuto;
    if(gom->t_GOM >= GOM_TMAX){
        gom->stato = COOLING;
        gom->tick_cooling_rimasti = GOM_COOLING_TIME;
        if (gom->tick_cooling_rimasti <= 0) {
            gom->stato = IDLE;
            gom->t_GOM = param.temperatura_ambiente_iniziale;
        }
    }
}
pezzo *gom_unload_and_evaluate(stazione_GOM *gom, int tick) {
    if (gom->stato == BUSY && gom->tick_lavorazione_rimasti <= 0) {
        gom->stato = IDLE;
        pezzo *p = gom->pezzo_in_lavorazione;
        gom->pezzo_in_lavorazione = NULL;

        p->ts.uscita = tick;
        p->tempo_gom_effettivo = tick - p->ts.inizio_gom;
        p->lead_time = tick - p->ts.ingresso;

        p->stato = gom_evaluate(p);
        
        return p;
    }
    return NULL;
}

piece_status gom_evaluate(pezzo *p){
    p->deviazione_gom = random_float(GOM_DEV_MIN, GOM_DEV_MAX);
    if(p->deviazione_gom >= p->valori_nom.deviazione_max_gom)
        return SCRAP;
    else return OK;
}

void quality_control ( int *ok, int *scrap, pezzo *list_head, pezzo *p){
    if(p->stato == OK){
        (*ok)++;
    }
    else{
        (*scrap)++;
        add_pezzo_next_in_production(list_head, p);
    }
}