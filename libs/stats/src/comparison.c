#include "comparison.h"
#include <string.h>

// direzione di "bonta'" di una metrica: dice quale valore e' preferibile
typedef enum {
    PIU_ALTO_MEGLIO, // es. pezzi completati: piu' alto = politica migliore
    PIU_BASSO_MEGLIO // es. lead time, ritardi, scarto: piu' basso = migliore
} direzione_t;

// riempie una riga del confronto e determina il vincitore in base alla direzione.
// static = visibile solo dentro questo file (dettaglio interno, non parte dell'API)
static void riempi_riga(riga_confronto_t *r, const char *nome,
                        double a, double b, direzione_t dir) {
    r->nome_metrica = nome;
    r->valore_a     = a;
    r->valore_b     = b;
    r->delta        = b - a;

    if (a == b) {
        r->vincitore = PARI;
    } else if (dir == PIU_ALTO_MEGLIO) {
        r->vincitore = (b > a) ? VINCE_B : VINCE_A;
    } else { // PIU_BASSO_MEGLIO
        r->vincitore = (b < a) ? VINCE_B : VINCE_A;
    }
}

// copia una stringa in un buffer di dimensione fissa garantendo il terminatore
static void copia_nome(char *dst, size_t dim, const char *src) {
    strncpy(dst, src, dim - 1);
    dst[dim - 1] = '\0';
}

int comparison_diff(const metriche_t *m1, const char *nome_a,
                    const metriche_t *m2, const char *nome_b,
                    confronto_t *out) {
    if (m1 == NULL || m2 == NULL || nome_a == NULL || nome_b == NULL || out == NULL)
        return -1;

    copia_nome(out->nome_a, sizeof(out->nome_a), nome_a);
    copia_nome(out->nome_b, sizeof(out->nome_b), nome_b);

    int i = 0;
    riempi_riga(&out->righe[i++], "Pezzi completati",     m1->pezzi_completati,     m2->pezzi_completati,     PIU_ALTO_MEGLIO);
    riempi_riga(&out->righe[i++], "Pezzi in ritardo",     m1->pezzi_in_ritardo,     m2->pezzi_in_ritardo,     PIU_BASSO_MEGLIO);
    riempi_riga(&out->righe[i++], "Rilavorazioni",        m1->rilavorazioni,        m2->rilavorazioni,        PIU_BASSO_MEGLIO);
    riempi_riga(&out->righe[i++], "Tasso scarto",         m1->tasso_scarto,         m2->tasso_scarto,         PIU_BASSO_MEGLIO);
    riempi_riga(&out->righe[i++], "Lead time medio",      m1->lead_time_medio,      m2->lead_time_medio,      PIU_BASSO_MEGLIO);
    riempi_riga(&out->righe[i++], "Blocchi buffer pieno", m1->blocchi_buffer_pieno, m2->blocchi_buffer_pieno, PIU_BASSO_MEGLIO);
    riempi_riga(&out->righe[i++], "Tick totali",          m1->tick_totali,          m2->tick_totali,          PIU_BASSO_MEGLIO);

    out->n_righe = i;
    return 0;
}
