#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

struct pezzo *buffer[BUFFER_SIZE];

  /*for(int i = 0; i<BUFFER_SIZE-1; i++){ //inizializza tutti i campi di id_pezzo a 0
        buffer[i].id_pezzo = 0;
        buffer[i].ID_ordine = 0;
        buffer[i].priorità = 0;
    }*/



void new_item(struct pezzo *p){
    if(is_full())
        printf("Error: buffer is full\n");

    else{
        for(int i = 0; i<BUFFER_SIZE-1; i++){
            if(buffer[i].id_pezzo == 0){
                buffer[i] = p;
                break;
            }
        }
    }
}

struct pezzo *take_item(){
    if(is_empty())
        printf("Error: buffer is empty\n");
    
    else{
        struct pezzo *in_uscita = buffer[0];
        for(int i = 0; i<BUFFER_SIZE-2; i++)
            buffer[i] = buffer[i+1];

        buffer[BUFFER_SIZE-1] = 0
        return in_uscita;
    }
}

bool is_full(){
    if buffer[BUFFER_SIZE-1].id_pezzo == 0
        return false;

    else
        return true;
}

bool is_empty(){
    if buffer[0].id_pezzo == 0
        return true;

    else
        return false;
}