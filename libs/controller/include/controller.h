#ifndef CONTROLLER_H
#define CONTROLLER_H

#define TEMPO_GOM 2
#include "cella.h"

/*Esegue il controllo logico di tutta la cella ricevendo in input la struttura cella_meccatronica.
Gestisce il flusso dei pezzi attraverso le stazioni di lavorazione e i buffer, provvedendo ad aggiornare 
statistiche e valutare la bontà dei pezzi eventualmente riinserendoli in produzione. Da chiamare DOPO il parsing*/
void controller(cella_meccatronica *c);

#endif