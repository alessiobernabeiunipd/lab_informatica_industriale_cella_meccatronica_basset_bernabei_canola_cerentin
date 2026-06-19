#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

struct pezzo *buffer[BUFFER_SIZE] = { NULL };

  /*for(int i = 0; i<BUFFER_SIZE-1; i++){ //inizializza tutti i campi di id_pezzo a 0
        buffer[i].id_pezzo = 0;
        buffer[i].ID_ordine = 0;
        buffer[i].priorità = 0;
    }*/



void new_item(struct pezzo *p){
    if(is_full())
        return;

    for(int i = 0; i < BUFFER_SIZE; i++){
        if(buffer[i] == NULL){
            buffer[i] = p;
            p->next = NULL;
            return;
        }
    }
}

struct pezzo *take_item(){
    if(is_empty())
        return NULL;

    struct pezzo *in_uscita = buffer[0];
    for(int i = 0; i < BUFFER_SIZE-1; i++)
        buffer[i] = buffer[i+1];

    buffer[BUFFER_SIZE-1] = NULL;
    return in_uscita;
}

bool is_full(){
    for(int i = 0; i < BUFFER_SIZE; i++){
        if(buffer[i] == NULL)
            return false;
    }
    printf("Error: buffer is full\n");
    return true;
}

bool is_empty(){
    if(buffer[0] == NULL){
        printf("Error: buffer is empty\n");
        return true;
    }

    return false;
}