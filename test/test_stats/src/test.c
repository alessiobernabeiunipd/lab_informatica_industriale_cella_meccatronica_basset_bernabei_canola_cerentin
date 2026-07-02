#include "unity.h"
#include "metrics.h"
#include "types.h"

// tutti i test condividono stessa output, acc e lead_time_arr 
// => porre attenzione al reset tra un test e l'altro per far partire ogni test pulito
// tutti i test hanno la stessa inizializzazione; per init diverso, faccio metrics_destroy

void setUp(void) {
    metrics_init(10); // capacità arbitraria
}
void tearDown(void) {
    metrics_destroy();
}  
// VERIFICO INIZIALIZZAZIONE E DESTROY
// test per vedere se init azzera ogni campo
void test_init_azzera_tutti_i_campi(void){
    const metriche_t *m = metrics_get();
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_INT(10, m->pezzi_totali);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_scartati);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_in_ritardo);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_non_finiti);
    TEST_ASSERT_EQUAL_INT(0, m->tick_totali);
    TEST_ASSERT_EQUAL_INT(0, m->blocchi_buffer_pieno);
    TEST_ASSERT_EQUAL_INT(0, m->errori_simulazione);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tasso_scarto);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->lead_time_medio);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tempo_laminazione_medio);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tempo_pressa_medio);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tempo_gom_medio);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->deviazione_gom_medio);
}
// test se tre destroy di fila crashano
void test_destroy(void){
    metrics_destroy();
    metrics_destroy();
    // tearDown chiama il terzo
    //se arrivo qui, no crash
}
// test per metrics_get prima di inizializzare
void test_get_dopo_destroy_ritorna_zero(void){
    metrics_destroy(); // rimuove l'init
    const metriche_t *m = metrics_get();
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_totali);
}
// VERIFICO INCREMENTO DEI CONTATORI CORRETTI
// definisco una funzione di appoggio utile per questi test:
static pezzo make_pezzo(int lead, int t_lam, int t_press, int t_gom, float dev){
    pezzo p = {0}; // azzera anche next, ts e valori_nom e tutto il resto della struct
    p.lead_time = lead;
    p.tempo_laminazione_effettivo = t_lam;
    p.tempo_pressa_effettivo = t_press;
    p.tempo_gom_effettivo = t_gom;
    p.deviazione_gom = dev;
    return p;
}
// test per incremento pezzi completati: contatore cresce e controllo che gli altri non vengano incrementati
void test_record_piece_done_incrementa_completati(void){
    pezzo p = make_pezzo(10, 5, 3, 2, 0.1f);
    record_piece_done(&p);
    record_piece_done(&p);
    record_piece_done(&p);
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_INT(3, m->pezzi_completati);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_scartati);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_in_ritardo);
}
// test per incremento pezzi scartati: contatore cresce e completati/in_ritardo restano a zero
void test_record_piece_scrap_incrementa_scartati(void)
{
    pezzo p = make_pezzo(0, 4, 2, 1, 0.0f);
    record_piece_scrap(&p);
    record_piece_scrap(&p);
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_INT(2, m->pezzi_scartati);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_completati);
    TEST_ASSERT_EQUAL_INT(0, m->pezzi_in_ritardo);
}

// test per incremento blocchi buffer pieno: contatore cresce e errori resta a zero
void test_record_block_incrementa_blocchi(void)
{
    record_block();
    record_block();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_INT(2, m->blocchi_buffer_pieno);
    TEST_ASSERT_EQUAL_INT(0, m->errori_simulazione);
}

// test per incremento errori simulazione: contatore cresce e blocchi resta a zero
void test_record_error_incrementa_errori(void)
{
    record_error();
    record_error();
    record_error();
    record_error();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_INT(4, m->errori_simulazione);
    TEST_ASSERT_EQUAL_INT(0, m->blocchi_buffer_pieno);
}
// LE FUNZIONI COMPUTE PRODUCONO METRICHE CORRETTE
// test lead time medio di due pezzi
void test_lead_time_medio_di_due_pezzi(void){
    pezzo p1 = make_pezzo(10, 0, 0, 0, 0);
    pezzo p2 = make_pezzo(20, 0, 0, 0, 0);
    record_piece_done(&p1);
    record_piece_done(&p2);
    metrics_compute_lead_time_medio();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(15.0f, m->lead_time_medio);
}
// test calcolo tempi medi stazione
void test_tempi_medi_stazione_includono_scarti(void){
    pezzo done = make_pezzo(0, 10, 20, 30, 0); //completato
    pezzo scrap = make_pezzo(0, 20, 40, 60, 0); //scartato
    record_piece_done(&done);
    record_piece_scrap(&scrap);
    metrics_compute_tempi_medi_stazione();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(15.0f, m->tempo_laminazione_medio);
    TEST_ASSERT_EQUAL_FLOAT(30.0f, m->tempo_pressa_medio);
    TEST_ASSERT_EQUAL_FLOAT(45.0f, m->tempo_gom_medio);
}
// test tasso di  scarto
void test_tasso_di_scarto(void){
    // all'inizio c'è metrics_init(10) da setUp, totali = 10
    pezzo s = make_pezzo(0, 0, 0, 0, 0);
    record_piece_scrap(&s);
    record_piece_scrap(&s);
    metrics_compute_tasso_scarto();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(0.2f, m->tasso_scarto);
}
//test deviazione GOM media (solo completati) => gli passo anche uno scarto
void test_deviazione_gom_media_solo_completati(void) {
    pezzo p1 = make_pezzo(0, 0, 0, 0, 0.1f);
    pezzo p2 = make_pezzo(0, 0, 0, 0, 0.3f);
    pezzo scrap_con_dev = make_pezzo(0, 0, 0, 0, 5.0f); // => se impatta nella media, si vede subito
    record_piece_done(&p1);
    record_piece_done(&p2);
    record_piece_scrap(&scrap_con_dev); // non deve influenzare la deviazione media
    metrics_compute_deviazione_gom_media();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(0.2f, m->deviazione_gom_medio); // (0.1+0.3)/2
}
// verifico la gestione della divsione per 0 nei vari compute
void test_lead_time_medio_zero_completati(void){
    metrics_compute_lead_time_medio(); // mai chiamato record_piece_done
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->lead_time_medio);
}
void test_tempi_medi_stazione_zero_pezzi(void){
    metrics_compute_tempi_medi_stazione();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tempo_laminazione_medio);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tempo_pressa_medio);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tempo_gom_medio);
}
void test_deviazione_gom_zero_completati(void) {
    metrics_compute_deviazione_gom_media();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->deviazione_gom_medio);
}
void test_tasso_scarto_zero_totali(void) {
    metrics_destroy();
    metrics_init(0);   // forziamo totali = 0
    metrics_compute_tasso_scarto();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_FLOAT(0.0f, m->tasso_scarto);
}
// TEST SU ARRAY LEAD TIME E REALLOC => testo in modo indiretto verificando che
// no crash quando realloc e che il lead time medio sia corretto sui pezzi
void test_array_lead_time_capacita_iniziale(void){
    // setUp ha fatto init(10), quindi capacità 10
    pezzo p = make_pezzo(7, 0, 0, 0, 0);
    for (int i = 0; i < 10; i++)
    {
        record_piece_done(&p);
    }
    metrics_compute_lead_time_medio();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_INT(10, m->pezzi_completati);
    TEST_ASSERT_EQUAL_FLOAT(7.0f, m->lead_time_medio);
} //realloc non dovrebbe scattare, in caso vedo una media strana
void test_array_lead_time_realloc(void){
    metrics_destroy();
    metrics_init(2); // capacità piccola intenzionalmente
    pezzo p = make_pezzo(5, 0, 0, 0, 0);
    for (int i=0; i<10; i++){
        record_piece_done(&p); // forza realloc
    }
    metrics_compute_lead_time_medio();
    const metriche_t *m = metrics_get();
    TEST_ASSERT_EQUAL_INT(10, m->pezzi_completati);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, m->lead_time_medio);
}
// ipotesi test n pz csv = 0 ma poi ne arrivano 3
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_azzera_tutti_i_campi);
    RUN_TEST(test_destroy);
    RUN_TEST(test_get_dopo_destroy_ritorna_zero);
    RUN_TEST(test_record_piece_done_incrementa_completati);
    RUN_TEST(test_record_piece_scrap_incrementa_scartati);
    RUN_TEST(test_record_block_incrementa_blocchi);
    RUN_TEST(test_record_error_incrementa_errori);
    RUN_TEST(test_lead_time_medio_di_due_pezzi);
    RUN_TEST(test_tempi_medi_stazione_includono_scarti);
    RUN_TEST(test_tasso_di_scarto);
    RUN_TEST(test_deviazione_gom_media_solo_completati);
    RUN_TEST(test_lead_time_medio_zero_completati);
    RUN_TEST(test_tempi_medi_stazione_zero_pezzi);
    RUN_TEST(test_deviazione_gom_zero_completati);
    RUN_TEST(test_tasso_scarto_zero_totali);
    RUN_TEST(test_array_lead_time_capacita_iniziale);
    RUN_TEST(test_array_lead_time_realloc);
    return UNITY_END();
}