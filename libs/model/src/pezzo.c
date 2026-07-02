#include "pezzo.h"


// funzione ausiliaria per add_pezzo, aggiunge il pezzo in coda alla lista
static void add_other_pezzo( pezzo *list_head, pezzo *to_add){
    while(list_head->next != NULL){
        list_head = list_head->next;
    }
    list_head->next = to_add;
}

void add_pezzo( pezzo **list_head, pezzo *to_add){
    to_add->next = NULL;                                    //valuta se la testa punta a null (collegamento primo pezzo), altrimenti chiama fun ausiliaria
    if(*list_head == NULL){
        *list_head = to_add;
        return;
    }
    add_other_pezzo(*list_head, to_add);
}

void add_pezzo_next_in_production( pezzo **list_head, pezzo *to_add){           
    pezzo *previous = last_pezzo_with_status(*list_head, TRAVELING);
    pezzo *p = copy_pezzo(to_add);
    if(p == NULL) return;
    reset_pezzo(p);
    if(previous == NULL){
        add_pezzo(list_head,p);
        return;
    }
    p->next = previous->next;
    previous->next = p;
}

pezzo *first_pezzo_with_status(pezzo *list_head, piece_status status){
    while(list_head != NULL && list_head->stato != status){
        list_head = list_head->next;
    }
    return list_head;
}

//attraversa tutta la lista tenendosi traccia dell'ultimo nodo trovato con un certo status
pezzo *last_pezzo_with_status(pezzo *list_head, piece_status status) {      
    pezzo *last = NULL;
    while (list_head != NULL) {
        if (list_head->stato == status) {
            last = list_head;
        }
        list_head = list_head->next;
    }
    return last;
}

pezzo *new_pezzo(){
    pezzo *new = malloc(sizeof(pezzo));
    if(new == NULL) return NULL;
    reset_pezzo(new);
    new->id_pezzo = 0;
    new->ID_ordine = 0;
    new->priorità = 0;
    new->deadline_ticks = 0;
    new->valori_nom.deviazione_max_gom = 0;
    new->valori_nom.t_gom = 0;
    new->valori_nom.t_laminazione_nominale = 0;
    new->valori_nom.t_pressa_nominale = 0;
    return new;
}

void modify_status(pezzo *p, piece_status new_status){
    p->stato = new_status;
}

pezzo *copy_pezzo(pezzo *p){
    pezzo *result = malloc(sizeof(pezzo));
    if(result == NULL) return NULL;             //protects against malloc failure, propagates error to caller
    *result = *p;
    result->next = NULL;                        //to copy a single piece and not the rest of the list after it
    return result;
}

void reset_pezzo(pezzo *p){
    p->stato = CREATED;
    p->ts.ingresso=0;
    p->ts.inizio_laminazione=0;
    p->ts.inizio_pressa=0;
    p->ts.inizio_gom=0;
    p->ts.uscita=0;
    p->tempo_laminazione_effettivo=0;
    p->tempo_pressa_effettivo=0;
    p->tempo_gom_effettivo=0;
    p->lead_time=0;
    p->deviazione_gom=0;
    p->next = NULL;
}

piece_status get_status(pezzo *p){
    return p->stato;
}

void set_nominal_times(pezzo *p, valori_nominali *v){
    if (v == NULL) return;
    p->valori_nom = v[p->id_pezzo - 1];
}