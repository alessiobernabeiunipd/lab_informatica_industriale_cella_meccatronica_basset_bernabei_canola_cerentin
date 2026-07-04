#ifndef UTILS_H
#define UTILS_H

#include "types.h"
//funzioni di generica utilità
//funzione per convertire un tipo enum in una stringa leggibile
const char* status_pezzo_to_string (piece_status s);
const char* status_station_to_string(station_status s);
float random_float(float min, float max);

// Variante deterministica di random_float: usa e aggiorna uno stato PRNG passato dal chiamante
// (LCG). A parità di stato iniziale produce sempre la stessa sequenza, indipendentemente
// dall'ordine con cui viene invocata: serve per rendere confrontabili run diverse (Common Random Numbers).
float random_float_seeded(unsigned int *state, float min, float max);

#endif