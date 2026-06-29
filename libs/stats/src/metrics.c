#include "metrics.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int somma_lead_time;
    int somma_t_laminazione;
    int somma_t_pressa;
    int somma_t_gom;
    float somma_deviazione_gom;
} accumulatori_t;

static accumulatori_t *acc = NULL;
static int *lead_times_arr = NULL;
static int lead_times_capacity = 0;
static int lead_times_count = 0; // serve a capire quanti elementi sono occupati nell'array
static metriche_t output;

const metriche_t *metrics_get(void){
    return &output;
}

int metrics_init(int n_pezzi_csv){
    memset(&output, 0, sizeof(output));
    output.pezzi_totali = n_pezzi_csv;
    acc = calloc(1, sizeof(accumulatori_t)); // calloc mi evita di fare memset
    if (acc == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria per accumulatori_t\n");
        return -1; 
    }
    if (n_pezzi_csv > 0){
    lead_times_arr = malloc(sizeof(int) * n_pezzi_csv);
    lead_times_capacity = n_pezzi_csv;
    lead_times_count = 0;
    if (lead_times_arr == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria per lead_times_arr\n");
        free(acc);
        return -1;
    }
}
    return 0;
}

void metrics_destroy(void){
    free(acc);
    acc = NULL;
    free(lead_times_arr);
    lead_times_arr = NULL;
    lead_times_capacity = 0;
    lead_times_count = 0;
}

void record_piece_done(const pezzo *p){
    output.pezzi_completati++;
    acc->somma_lead_time += p->lead_time;
    acc->somma_t_laminazione += p->tempo_laminazione_effettivo;
    acc->somma_t_pressa += p->tempo_pressa_effettivo;
    acc->somma_t_gom += p->tempo_gom_effettivo;
    acc->somma_deviazione_gom += p->deviazione_gom;

    if (lead_times_count >= lead_times_capacity) {
        int nuova_capacita = (lead_times_capacity == 0) ? 1 : lead_times_capacity * 2;
        int *tmp = realloc(lead_times_arr, sizeof(int) * nuova_capacita);
        if (tmp == NULL) {
            fprintf(stderr, "Errore nell'allocazione della memoria per lead_times_arr\n");
            return;
        }
        lead_times_arr = tmp;
        lead_times_capacity = nuova_capacita;
    }
    lead_times_arr[lead_times_count++] = p->lead_time;
}

void record_piece_scrap(const pezzo *p)
{
    output.pezzi_scartati++;
    acc->somma_lead_time += p->lead_time;
    acc->somma_t_laminazione += p->tempo_laminazione_effettivo;
    acc->somma_t_pressa += p->tempo_pressa_effettivo;
    acc->somma_t_gom += p->tempo_gom_effettivo;
}

void record_piece_late(const pezzo *p)
{
    output.pezzi_in_ritardo++;
}

void record_block(void)
{
    output.blocchi_buffer_pieno++;
}
void record_error(void){
    output.errori_simulazione++;
}