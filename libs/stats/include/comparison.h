#ifndef comparison_h
#define comparison_h
#include "types.h"

// ho definito strutture e tipi in types-h per evitare interdipendenze tra librerie

// confronta le metriche di due simulazioni (m1 = politica nome_a, m2 = politica nome_b)
// e riempie *out con valori, delta e vincitore per ogni metrica interessante.
// non modifica lo stato interno di metrics: lavora su due metriche_t gia' pronte.
// ritorna 0 se ok, -1 se un puntatore in ingresso e' NULL.
int comparison_diff(const metriche_t *m1, const char *nome_a,
                    const metriche_t *m2, const char *nome_b,
                    confronto_t *out);
#endif
