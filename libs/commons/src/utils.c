#include "utils.h"    
#include "logger.h"

const char* status_pezzo_to_string(piece_status s){
    switch (s){
    case CREATED:           return "CREATED";
    case TRAVELING:         return "TRAVELING";
    case WAITING_INPUT:     return "WAITING_INPUT";
    case IN_LAMINATION:     return "IN_LAMINATION";
    case WAITING_PRESS:     return "WAITING_PRESS";
    case IN_PRESS:          return "IN_PRESS";
    case ON_CONVEYOR:       return "ON_CONVEYOR";
    case IN_GOM:            return "IN_GOM";
    case OK:                return "OK";
    case SCRAP:             return "SCRAP";
    case DONE:              return "DONE";
    default :               return "ERROR";
    }
}

const char* status_station_to_string(station_status s){
    switch (s){
        case IDLE:          return "IDLE";
        case BUSY:          return "BUSY";
        case COOLING:       return "COOLING";
        default:            return "ERROR";
    }
}

float random_float(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

float random_float_seeded(unsigned int *state, float min, float max) {
    // LCG (Numerical Recipes): avanza lo stato e ne usa i bit alti per un valore in [0, 1).
    *state = (*state) * 1664525u + 1013904223u;
    float unit = (float)(*state >> 8) / (float)(1u << 24);
    return min + unit * (max - min);
}
