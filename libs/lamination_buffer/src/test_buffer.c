#include <stdio.h>
#include <stdbool>

#include "buffer.h"

    typedef struct pezzo{
    int id_pezzo = 1;
    int ID_ordine = 1;              // per identificare pezzi che appartengono allo stesso ordine. Utile per le stats finali per fare cfr su totale pezzi ordine vs pezzi prodotti e calcolare scarto
    int priorità = 1; 
    int deadline_ticks = 0;         // entro quando devo completare il pezzo? sono nei tempi richiesti?
    valori_nominali *tempi_nom; // pointer into the catalogue array 
    piece_status stato;
    timestamps ts;
    int tempo_laminazione_effettivo = 0;
    int tempo_pressa_effettivo = 0; 
    int lead_time = 0; 
    float deviazione_gom = 0;        //inizializzato a zero
    struct Pezzo *next;
    }pezzo;

int main(){

    struct pezzo *head = NULL;

    for(i = 0; i < BUFFER_SIZE-1; i++){
        struct pezzo *nuovo_pezzo;

        nuovo_pezzo = malloc(sizeof(struct pezzo));
        if (nuovo_pezzo == NULL){
            printf("Error: malloc() failed in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i+1;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;
    }

    
}