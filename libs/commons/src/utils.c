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

void print_pezzi(const pezzo *list_head) {
    log_info(0, "=== ELENCO DEI PEZZI DA PRODURRE ===");
    const pezzo *curr_print = list_head;
    while (curr_print != NULL) {
        log_info_f(0, "  Pezzo ID: %02d | Ordine ID: %d | Priorita: %d | Deadline: %4d tick | Lam Nom: %2d tick | Pres Nom: %2d tick | GOM Nom: %2d tick | Max Dev GOM: %.2f", 
                   curr_print->id_pezzo, 
                   curr_print->ID_ordine, 
                   curr_print->priorità, 
                   curr_print->deadline_ticks, 
                   curr_print->valori_nom.t_laminazione_nominale, 
                   curr_print->valori_nom.t_pressa_nominale, 
                   curr_print->valori_nom.t_gom, 
                   curr_print->valori_nom.deviazione_max_gom / 100.0f);
        curr_print = curr_print->next;
    }
    log_info(0, "====================================");
}
