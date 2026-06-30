#include "metrics.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    long somma_lead_time; // evito overflow
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
    memset(&output, 0, sizeof(output)); // reset output
}

void record_piece_done(const pezzo *p){
    output.pezzi_completati++;
    acc->somma_lead_time += p->lead_time;
    acc->somma_t_laminazione += p->tempo_laminazione_effettivo;
    acc->somma_t_pressa += p->tempo_pressa_effettivo;
    acc->somma_t_gom += p->tempo_gom_effettivo;
    acc->somma_deviazione_gom += p->deviazione_gom;

    if (lead_times_count >= lead_times_capacity) {
        int nuova_capacita = (lead_times_capacity == 0) ? 1 : lead_times_capacity * 2; // n.c parte da 1 quando lead_capacity è 0
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
    acc->somma_t_laminazione += p->tempo_laminazione_effettivo;
    acc->somma_t_pressa += p->tempo_pressa_effettivo;
    acc->somma_t_gom += p->tempo_gom_effettivo;
    // la deviazione del gom va solo sui completati, come il lead time
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
void metrics_compute_lead_time_medio(void){
    if(output.pezzi_completati == 0) return; // no divisioni per 0
    output.lead_time_medio = (float)acc->somma_lead_time/output.pezzi_completati;
    // la divisione in C tronca a intero => mi serve uno dei due operandi in float prima delle divisone
}
void metrics_compute_tempi_medi_stazione (void) {
    if (output.pezzi_completati + output.pezzi_scartati == 0) return;
    output.tempo_laminazione_medio = (float)acc->somma_t_laminazione/(output.pezzi_completati+output.pezzi_scartati);
    output.tempo_gom_medio = (float)acc->somma_t_gom/(output.pezzi_completati+output.pezzi_scartati);
    output.tempo_pressa_medio = (float)acc->somma_t_pressa/(output.pezzi_completati+output.pezzi_scartati);
}
void metrics_compute_tasso_scarto(void){
    if(output.pezzi_totali == 0) return;
    output.tasso_scarto = (float)output.pezzi_scartati/output.pezzi_totali;
}
void metrics_compute_deviazione_gom_media(void){
    if(output.pezzi_completati == 0) return;
    output.deviazione_gom_medio = acc->somma_deviazione_gom/output.pezzi_completati;
}