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

int main (void){
    UNITY_BEGIN();
    RUN_TEST(test_simulazione_vuota);
    UNITY_END();
}