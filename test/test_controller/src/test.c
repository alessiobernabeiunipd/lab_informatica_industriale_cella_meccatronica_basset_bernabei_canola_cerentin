#include "controller.h"
#include "cella.h"
#include "logger.h"
#include "metrics.h"
#include "unity.h"
#include "types.h"

// cella condivisa tra setUp/tearDown e i singoli test
static cella_meccatronica *cella = NULL;

void setUp(void){
    // parametri di default per i test: buffer piccoli, politica fcfs, durata contenuta.
    // I singoli test possono sovrascrivere cella->param prima di chiamare controller().
    parametri_simulazione param;
    memset(&param, 0, sizeof(param));
    param.max_area_pressa = 1000.0f;
    param.capacita_buffer_laminazione = 5;
    param.capacita_buffer_pressa = 5;
    param.capacita_buffer_gom = 5;
    strcpy(param.politica_controllo, "fcfs");
    param.temperatura_ambiente_iniziale = 20.0f;
    param.temperatura_incremento_minuto = 2.0f;
    param.durata_simulazione_max = 50;

    logger_init("/dev/null"); // inizializzazione per non scrivere su file durante i test
    cella = init_cella(param);
    TEST_ASSERT_NOT_NULL(cella);
    // NB: metrics_init() NON va chiamato qui: lo fa gia' controller() internamente,
    // e una doppia init causerebbe un leak (metrics_init rialloca senza liberare).
}
void tearDown(void){
    logger_close();
    metrics_destroy();   // libera acc/lead_times_arr allocati da controller()
    cella_destroy(cella);
    cella = NULL;
}

// test simulazione vuota
void test_simulazione_vuota(void){
    controller(cella);
    TEST_ASSERT_EQUAL_INT(0, cella->metrics.pezzi_completati);
    TEST_ASSERT_EQUAL_INT(0, cella->metrics.pezzi_scartati);
}
// verifico che un pezzo venga completato correttamente
void test_pezzo_completo(void){
    pezzo *p = new_pezzo(); 
    p->valori_nom.t_laminazione_nominale = 1;
    p->valori_nom.t_pressa_nominale = 1;
    p->valori_nom.t_gom = 1;
    // dato che gom_evaluate assegna deviazione random, imposto manualmente
    p->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
    add_pezzo(&cella->list_head, p);
    controller(cella);
    TEST_ASSERT_EQUAL_INT(1, cella->metrics.pezzi_completati);
    TEST_ASSERT_EQUAL_INT(0, cella->metrics.pezzi_scartati);
}
// verifico che con FCFS l'AGV prenda i pezzi nell'ordine corretto
void test_fcfs(void){
    pezzo *p1 = new_pezzo();
    pezzo *p2 = new_pezzo();
    pezzo *p3 = new_pezzo();
    p1->id_pezzo = 1;
    p2->id_pezzo = 2;
    p3->id_pezzo = 3;
    p1->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
    p2->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
    p3->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
    add_pezzo(&cella->list_head, p1);
    add_pezzo(&cella->list_head, p2);
    add_pezzo(&cella->list_head, p3);
    controller(cella);
    TEST_ASSERT_TRUE(p1->ts.ingresso < p2->ts.ingresso);
    TEST_ASSERT_TRUE(p2->ts.ingresso < p3->ts.ingresso);
    TEST_ASSERT_EQUAL_INT(3, cella->metrics.pezzi_completati);
}
// test politica priorità
void test_priorita(void){
    pezzo *p1 = new_pezzo();
    pezzo *p2 = new_pezzo();
    pezzo *p3 = new_pezzo();
    p1->id_pezzo = 1;
    p2->id_pezzo = 2;
    p3->id_pezzo = 3;
    p1->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
    p2->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
    p3->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
    p1->priorità = 3;
    p2->priorità = 2;
    p3->priorità = 1;
    strcpy(cella->param.politica_controllo, "priorita");
    add_pezzo(&cella->list_head, p1);
    add_pezzo(&cella->list_head, p2);
    add_pezzo(&cella->list_head, p3);
    controller(cella);
    TEST_ASSERT_TRUE(p3->ts.ingresso < p2->ts.ingresso);
    TEST_ASSERT_TRUE(p2->ts.ingresso < p1->ts.ingresso);
    TEST_ASSERT_EQUAL_INT(3, cella->metrics.pezzi_completati);
}
// test buffer pieno => il controller prima di scaricare, controlla se c'è spazio nel buffer
// blocco GOM così buffer a monte si riempie e pressa non scarica
void test_buffer_pieno(void){
    for (int i = 0; i < 10; i++){
        pezzo *p = new_pezzo();
        p->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MAX * 100);
        p->valori_nom.t_gom = 1000; // gom lento
        p->valori_nom.t_laminazione_nominale = 1; // monte veloce
        p->valori_nom.t_pressa_nominale = 1; // monte veloce
        add_pezzo(&cella->list_head, p);
    }
    controller (cella);
    TEST_ASSERT_TRUE(metrics_get()->blocchi_buffer_pieno > 0);
    //il blocco non deve perdere pezzi, sono tutti ancora in lista
    int conteggio = 0;
    for (pezzo *curr = cella->list_head; curr != NULL; curr = curr->next)
        conteggio++;
    TEST_ASSERT_EQUAL_INT(10, conteggio);
}
// test scarto e rilavorazione => genero una catena di scarti fino a fine simulazione
void test_scarto_rilavorazione(void){
    pezzo *p = new_pezzo();
    p->valori_nom.deviazione_max_gom = (int)(GOM_DEV_MIN * 100);
    p->valori_nom.t_laminazione_nominale = 1;
    p->valori_nom.t_pressa_nominale = 1;
    p->valori_nom.t_gom = 1;
    add_pezzo(&cella->list_head, p);
    controller(cella);
    TEST_ASSERT_TRUE(cella->metrics.pezzi_scartati > 0);
    TEST_ASSERT_EQUAL_INT(0, cella->metrics.pezzi_completati);
    // invariante: partito da 1 pezzo, ogni scarto ha aggiunto esattamente una copia di rilavorazione
    int conteggio = 0;
    for (pezzo *curr = cella->list_head; curr != NULL; curr = curr->next)
        conteggio++;
    TEST_ASSERT_EQUAL_INT(1 + cella->metrics.pezzi_scartati, conteggio);
}
int main (void){
    UNITY_BEGIN();
    RUN_TEST(test_simulazione_vuota);
    RUN_TEST(test_pezzo_completo);
    RUN_TEST(test_fcfs);
    RUN_TEST(test_priorita);
    RUN_TEST(test_buffer_pieno);
    RUN_TEST(test_scarto_rilavorazione);
    UNITY_END();
}