#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

    typedef struct pezzo{
    int id_pezzo;
    int ID_ordine;              // per identificare pezzi che appartengono allo stesso ordine. Utile per le stats finali per fare cfr su totale pezzi ordine vs pezzi prodotti e calcolare scarto
    int priorità; 
    int deadline_ticks;         // entro quando devo completare il pezzo? sono nei tempi richiesti?
    //valori_nominali *tempi_nom; // pointer into the catalogue array 
    //piece_status stato;
    //timestamps ts;
    int tempo_laminazione_effettivo;
    int tempo_pressa_effettivo; 
    int lead_time; 
    float deviazione_gom;        //inizializzato a zero
    struct Pezzo *next;
    }pezzo;

int main(){

    struct pezzo *head = NULL;

    for(int i = 0; i < BUFFER_SIZE-1; i++){
        struct pezzo *nuovo_pezzo;

        nuovo_pezzo = malloc(sizeof(struct pezzo));
        if (nuovo_pezzo == NULL){
            printf("Error: malloc() failed in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i+1;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

        new_item(nuovo_pezzo);
    }

    
}