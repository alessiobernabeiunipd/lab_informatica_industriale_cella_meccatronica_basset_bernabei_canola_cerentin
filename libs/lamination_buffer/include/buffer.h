/*Take the element from the struct to insert it in the array.
 *Put the element in the first free spot.
 *If there is no space left in the buffer send an error message
 *to the controller
 *
 *Function needed:
 *New item in the buffer
 *Remove item from the buffer
 *Check the buffer state
*/

#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 6

//pezzo che viene lavorato, una struct definisce un pezzo singolo. 
//linked list
typedef struct pezzo{
    int id_pezzo;
    int ID_ordine;              // per identificare pezzi che appartengono allo stesso ordine. Utile per le stats finali per fare cfr su totale pezzi ordine vs pezzi prodotti e calcolare scarto
    int priorità; 
    int deadline_ticks;         // entro quando devo completare il pezzo? sono nei tempi richiesti?
    valori_nominali *tempi_nom; // pointer into the catalogue array 
    piece_status stato;
    timestamps ts;
    int tempo_laminazione_effettivo;
    int tempo_pressa_effettivo; 
    int lead_time; 
    float deviazione_gom;        //inizializzato a zero
    struct Pezzo *next;
}pezzo;

/*Adding new item to the buffer*/
void new_item(struct pezzo *p);

/*Removing item from the buffer*/
struct pezzo *take_item();

/*Checking the buffer state*/
bool is_full();

#endif