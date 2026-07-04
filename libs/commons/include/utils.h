#ifndef UTILS_H
#define UTILS_H

#include "types.h"
//funzioni di generica utilità
//funzione per convertire un tipo enum in una stringa leggibile
const char* status_pezzo_to_string (piece_status s);
const char* status_station_to_string(station_status s);
float random_float(float min, float max);

#endif 