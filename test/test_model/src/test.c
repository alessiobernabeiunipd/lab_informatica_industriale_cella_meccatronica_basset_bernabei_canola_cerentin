#include "cella.h"
#include "unity.h"

static void free_lista_pezzi(pezzo *head){
    pezzo *current = head;
    while(current != NULL){
        pezzo *next = current->next;
        free(current);
        current = next;
    }
}

void setUp(void) {}
void tearDown(void) {}

void test_modify_status(){
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    TEST_ASSERT_EQUAL_STRING("CREATED", status_pezzo_to_string(a->stato));
    TEST_ASSERT_EQUAL_INT(0, a->ts.ingresso);
    modify_status(a, IN_PRESS);
    TEST_ASSERT_EQUAL_STRING("IN_PRESS", status_pezzo_to_string(a->stato));
    free(a);
}

void test_copy_pezzo(){
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    a->id_pezzo = 3;
    a->stato = IN_GOM;
    a->ts.ingresso = 100;
    pezzo *copy = copy_pezzo(a);
    TEST_ASSERT_EQUAL_STRING("IN_GOM", status_pezzo_to_string(copy->stato));
    TEST_ASSERT_EQUAL_INT(3, copy->id_pezzo);
    TEST_ASSERT_EQUAL_INT(100, copy->ts.ingresso);
    TEST_ASSERT_NULL(copy->next);
    free(a);
    free(copy);
}

void test_list(){
    pezzo *list_head = NULL;
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    pezzo *b = new_pezzo();
    reset_pezzo(b);
    pezzo *c = new_pezzo();
    reset_pezzo(c);
    modify_status(a, IN_LAMINATION);
    modify_status(b, IN_LAMINATION);
    modify_status(c, WAITING_INPUT);
    add_pezzo(&list_head, a);
    add_pezzo(&list_head, b);
    add_pezzo(&list_head, c);
    TEST_ASSERT_EQUAL_INT(b,a->next);
    TEST_ASSERT_EQUAL_INT(c,b->next);
    TEST_ASSERT_EQUAL_INT(a,list_head);
    free_lista_pezzi(list_head);
}

void test_first(){
    pezzo *list_head = NULL;
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    pezzo *b = new_pezzo();
    reset_pezzo(b);
    pezzo *c = new_pezzo();
    reset_pezzo(c);
    modify_status(a, IN_LAMINATION);
    modify_status(b, IN_LAMINATION);
    modify_status(c, WAITING_INPUT);
    add_pezzo(&list_head, a);
    add_pezzo(&list_head, b);
    add_pezzo(&list_head, c);

    pezzo *p = first_pezzo_with_status(list_head, WAITING_INPUT);
    pezzo *k = first_pezzo_with_status(list_head, SCRAP);
    TEST_ASSERT_EQUAL_INT(c, p);
    TEST_ASSERT_NULL(k);
    free_lista_pezzi(list_head);
}

void test_add(){
    pezzo *list_head = NULL;
    pezzo *a = new_pezzo();
    reset_pezzo(a);
    pezzo *b = new_pezzo();
    reset_pezzo(b);
    pezzo *c = new_pezzo();
    reset_pezzo(c);
    modify_status(a, IN_LAMINATION);
    modify_status(b, IN_LAMINATION);
    modify_status(c, TRAVELING);
    add_pezzo(&list_head, a);
    add_pezzo(&list_head, b);
    add_pezzo(&list_head, c);

    pezzo *d = new_pezzo();
    reset_pezzo(d);
    modify_status(d, SCRAP);
    d->id_pezzo=1;
    add_pezzo_next_in_production(&list_head, d);
    TEST_ASSERT_EQUAL_INT(1, c->next->id_pezzo);
    TEST_ASSERT_EQUAL_STRING("CREATED", status_pezzo_to_string(c->next->stato));

    pezzo *e = new_pezzo();
    reset_pezzo(e);
    e->id_pezzo=4;
    modify_status(e, SCRAP);
    add_pezzo_next_in_production(&list_head, e);
    TEST_ASSERT_EQUAL_INT(4, c->next->id_pezzo);
    TEST_ASSERT_EQUAL_STRING("CREATED", status_pezzo_to_string(c->next->stato));
    free_lista_pezzi(list_head); // libera a,b,c e le copie di d,e, ma non gli originali
    free(d);
    free(e);
}

void test_high_priority(){
    pezzo *list_head = NULL;
    for(int i=0; i<10; i++){
        pezzo *p = new_pezzo();
        reset_pezzo(p);
        p->id_pezzo = i;
        if(i%3==0){
            p->priorità = 1;
        }else{
            p->priorità = 0;
        }
        if(i%2==0){
            modify_status(p, IN_LAMINATION);
        }else{
            modify_status(p, WAITING_INPUT);
        }
        add_pezzo(&list_head, p);
    }

    // Convenzione: numero di priorità piu' basso = priorità maggiore, quindi high_priority
    // restituisce il MINIMO. Tra i pari (IN_LAMINATION) il primo con priorità 0 e' i=2;
    // tra i dispari (WAITING_INPUT) il primo con priorità 0 e' i=1.
    pezzo *p = high_priority(list_head, IN_LAMINATION);
    TEST_ASSERT_EQUAL_INT(0, p->priorità);
    TEST_ASSERT_EQUAL_INT(2, p->id_pezzo);
    pezzo *q = high_priority(list_head, WAITING_INPUT);
    TEST_ASSERT_EQUAL_INT(0, q->priorità);
    TEST_ASSERT_EQUAL_INT(1, q->id_pezzo);
    free_lista_pezzi(list_head);
}

void test_init_agv(){       //testa le funzioni di inizializzazione dell'AGV
    AGV *agv = init_agv();
    TEST_ASSERT_EQUAL_STRING("IDLE", status_station_to_string(agv->stato));
    TEST_ASSERT_EQUAL_INT(0, agv->tick_lavorazione_rimasti);
    TEST_ASSERT_NULL(agv->pezzo_in_lavorazione);
    int i = agv_is_free(agv);
    TEST_ASSERT_EQUAL_INT(1, i);
    agv->stato = BUSY;
    int j = agv_is_free(agv);
    TEST_ASSERT_EQUAL_INT(0, j);
    free(agv);
}

void test_get_mold(){
    AGV *agv = init_agv();
    pezzo *p = new_pezzo();
    agv_get_mold(agv, p, 10, 9);
    TEST_ASSERT_EQUAL_STRING("BUSY", status_station_to_string(agv->stato));
    TEST_ASSERT_EQUAL_INT(p, agv->pezzo_in_lavorazione);
    TEST_ASSERT_EQUAL_INT(20, agv->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(9, p->ts.ingresso);
    TEST_ASSERT_EQUAL_STRING("TRAVELING", status_pezzo_to_string(p->stato));
    agv_get_mold(agv, p, 1000, 1000);
    TEST_ASSERT_EQUAL_INT(p, agv->pezzo_in_lavorazione);
    TEST_ASSERT_EQUAL_INT(20, agv->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(9, p->ts.ingresso);
    free(agv);
    free(p);

}

void test_tick(){
    AGV *agv = init_agv();
    TEST_ASSERT_EQUAL_INT(0, agv->tick_lavorazione_rimasti);
    pezzo *p = new_pezzo();
    agv_get_mold(agv, p, 10, 9);
    agv_tick(agv);
    TEST_ASSERT_EQUAL_INT(19, agv->tick_lavorazione_rimasti);
    free(agv);
    free(p);
}

void test_agv_unload(){
    AGV *agv = init_agv();
    pezzo *p = new_pezzo();
    agv_get_mold(agv, p, 10, 9);
    pezzo *m = agv_unload(agv);
    TEST_ASSERT_NULL( m);
    agv->tick_lavorazione_rimasti = 0;
    pezzo *r = agv_unload(agv);
    TEST_ASSERT_EQUAL_INT(p, r);
    TEST_ASSERT_NULL( agv->pezzo_in_lavorazione);
    TEST_ASSERT_EQUAL_STRING("IDLE", status_station_to_string(agv->stato));
    free(agv);
    free(p);
}

void test_init_gom(){       //testa le funzioni di inizializzazione dell'AGV
    stazione_GOM *gom = init_gom();
    TEST_ASSERT_EQUAL_STRING("IDLE", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(0, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(0, gom->tick_cooling_rimasti);
    TEST_ASSERT_NULL(gom->pezzo_in_lavorazione);
    int i = gom_is_free(gom);
    TEST_ASSERT_EQUAL_INT(1, i);
    gom->stato = BUSY;
    int j = gom_is_free(gom);
    TEST_ASSERT_EQUAL_INT(0, j);
    free(gom);
}

void test_gom_tick(){
    stazione_GOM *gom = init_gom();
    gom->t_GOM = 20;
    pezzo *p = new_pezzo();
    p->valori_nom.t_gom = 10;
    gom_load(gom, p, 10);
    parametri_simulazione param;
    param.temperatura_ambiente_iniziale = 20;
    param.temperatura_incremento_minuto = 2;
    gom_tick(gom, param);
    TEST_ASSERT_EQUAL_STRING("BUSY", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(9, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(0, gom->tick_cooling_rimasti);
    TEST_ASSERT_EQUAL_INT(p, gom->pezzo_in_lavorazione);
    TEST_ASSERT_EQUAL_INT(22, gom->t_GOM);
    gom->t_GOM = 26;
    gom_tick(gom, param);
    TEST_ASSERT_EQUAL_STRING("COOLING", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(8, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(10, gom->tick_cooling_rimasti);
    gom_tick(gom, param);
    TEST_ASSERT_EQUAL_STRING("COOLING", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(8, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(9, gom->tick_cooling_rimasti);
    
    gom->tick_cooling_rimasti = 1;
    stazione_GOM *gom2 = init_gom();                                            //test per veriicare il ritorno a t_amb a fine cooling
    *gom2 = *gom;
    gom_tick(gom, param);
    TEST_ASSERT_EQUAL_STRING("BUSY", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(8, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(0, gom->tick_cooling_rimasti);
    TEST_ASSERT_EQUAL_INT(param.temperatura_ambiente_iniziale, gom->t_GOM);

    stazione_GOM *gom3 = init_gom();                                            //test per veriicare il ritorno a IDLE
    *gom3 = *gom;
    gom3->tick_lavorazione_rimasti = 0;
    gom_tick(gom, param);
    TEST_ASSERT_EQUAL_STRING("BUSY", status_station_to_string(gom->stato));
    free(gom);
    free(gom2);
    free(gom3);
    free(p);
}

void test_gom_unload_evaluate_1(){      // valuto condizione di scarico possibile
    stazione_GOM *gom = init_gom();
    pezzo *p = new_pezzo();
    parametri_simulazione param;
    param.temperatura_ambiente_iniziale = 20;
    param.temperatura_incremento_minuto = 2;
    p->valori_nom.deviazione_max_gom = GOM_DEV_MAX;
    p->valori_nom.t_gom = 1;
    gom_load(gom, p, 80);
    gom_tick(gom, param);
    p = gom_unload_and_evaluate(gom, 99);
    TEST_ASSERT_EQUAL_STRING("IDLE", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(0, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(0, gom->tick_cooling_rimasti);
    TEST_ASSERT_EQUAL_INT(99, p->ts.uscita);
    TEST_ASSERT_EQUAL_INT(99, p->lead_time);
    TEST_ASSERT_EQUAL_STRING("OK", status_pezzo_to_string(OK));
    free(gom);
    free(p);
}

void test_gom_unload_evaluate_2(){      //caso con scarico possibile, verifico che l'unload avvenga
    stazione_GOM *gom = init_gom();
    pezzo *p = new_pezzo();
    p->valori_nom.t_gom = 1;
    gom_load(gom, p, 1);
    gom->t_GOM = 26;
    parametri_simulazione param;
    param.temperatura_ambiente_iniziale = 20;
    param.temperatura_incremento_minuto = 2;
    gom_tick(gom, param);
    p = gom_unload_and_evaluate(gom, 99);
    TEST_ASSERT_EQUAL_STRING("COOLING", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(0, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(10, gom->tick_cooling_rimasti);
    gom_tick(gom, param);
    free(gom);
    free(p);
}

void test_gom_unload_evaluate_3(){          //caso in cui non avviene alcuno scarico, verifico che non avvenga nulla
    stazione_GOM *gom = init_gom();
    pezzo *p = new_pezzo();
    p->valori_nom.t_gom = 10;
    gom_load(gom, p, 22);
    gom->t_GOM = 26;
    parametri_simulazione param;
    param.temperatura_ambiente_iniziale = 20;
    param.temperatura_incremento_minuto = 2;
    gom_tick(gom, param);
    pezzo *r = gom_unload_and_evaluate(gom, 99);
    TEST_ASSERT_EQUAL_STRING("COOLING", status_station_to_string(gom->stato));
    TEST_ASSERT_EQUAL_INT(9, gom->tick_lavorazione_rimasti);
    TEST_ASSERT_EQUAL_INT(10, gom->tick_cooling_rimasti);
    TEST_ASSERT_NULL(r);
    TEST_ASSERT_EQUAL_STRING("IN_GOM", status_pezzo_to_string(p->stato));
    free(gom);
    free(p);
}

void test_quality_control_OK(){
    stazione_GOM *gom = init_gom();
    pezzo *p = new_pezzo();
    parametri_simulazione param;
    param.temperatura_ambiente_iniziale = 20;
    param.temperatura_incremento_minuto = 2;
    p->valori_nom.deviazione_max_gom = GOM_DEV_MAX;
    gom_load(gom, p, 1);
    gom_tick(gom, param);
    p = gom_unload_and_evaluate(gom, 99);     //genero un pezzo che so essere OK
    
    pezzo *list_head = NULL;
    int scrap = 0;
    int ok = 0;
    quality_control(&ok, &scrap, &list_head, p);
    TEST_ASSERT_EQUAL_INT(1, ok);
    free(gom);
    free(p);
    free_lista_pezzi(list_head);
}

void test_quality_control_SCRAP(){
    stazione_GOM *gom = init_gom();
    pezzo *p = new_pezzo();
    p->id_pezzo = 855;                          //inserisco un valore che posso tracciare
    parametri_simulazione param;
    param.temperatura_ambiente_iniziale = 20;
    param.temperatura_incremento_minuto = 2;
    p->valori_nom.deviazione_max_gom = GOM_DEV_MIN;
    gom_load(gom, p, 1);
    gom_tick(gom, param);
    p = gom_unload_and_evaluate(gom, 99);     //genero un pezzo che so essere SCRAP
    
    pezzo *list_head = NULL;
    int scrap = 0;
    int ok = 0;
    quality_control(&ok, &scrap, &list_head, p);
    TEST_ASSERT_EQUAL_INT(1, scrap);
    TEST_ASSERT_EQUAL_INT(855, list_head->id_pezzo);
    free(gom);
    free(p);
    free_lista_pezzi(list_head);
}

int main (void)
{
    UNITY_BEGIN();

    RUN_TEST(test_modify_status);
    RUN_TEST(test_copy_pezzo);
    RUN_TEST(test_list);
    RUN_TEST(test_first);
    RUN_TEST(test_add);
    RUN_TEST(test_high_priority);

    RUN_TEST(test_init_agv);
    RUN_TEST(test_get_mold);
    RUN_TEST(test_tick);
    RUN_TEST(test_agv_unload);

    RUN_TEST(test_init_gom);
    RUN_TEST(test_gom_tick);
    RUN_TEST(test_gom_unload_evaluate_1);
    RUN_TEST(test_gom_unload_evaluate_2);
    RUN_TEST(test_gom_unload_evaluate_3);
    RUN_TEST(test_quality_control_OK);
    RUN_TEST(test_quality_control_SCRAP);
   
    return UNITY_END();
}