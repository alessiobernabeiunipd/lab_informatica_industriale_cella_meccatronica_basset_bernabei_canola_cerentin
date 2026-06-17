#include <stdio.h>      // system headers with angle brackets
#include <string.h>
#include <stdlib.h>

#include "utils.h"    

const char* status_pezzo_to_string(piece_status s){
    switch (s){
    case CREATED:           return "CREATED";
    case TRAVELING:         return "TRAVELING"
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
        case BROKEN:        return "BROKEN";
        default:            return "ERROR";
    }
}

