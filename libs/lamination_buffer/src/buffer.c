#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"


void initialize(){
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = malloc(sizeof(struct pezzo));
        if (buffer[i] == NULL){
            printf("Error: malloc() failed in initialize()\n");
            exit(EXIT_FAILURE);
        }
        buffer[i]->id_pezzo = 0;
    }
}

void new_item(struct pezzo *p){
    if(is_full())
        return;

    else{
        for(int i = 0; i <= BUFFER_SIZE-1; i++){
            if(buffer[i]->id_pezzo == 0){
                buffer[i] = p;
                break;
            }
        }
    }
}

struct pezzo *take_item(){
    if(is_empty())
        return NULL;
    
    else{
        struct pezzo *in_uscita = buffer[0];
        for(int i = 0; i <= BUFFER_SIZE-2; i++)
            buffer[i] = buffer[i+1];

        buffer[BUFFER_SIZE-1]->id_pezzo = 0;
        return in_uscita;
    }
}

bool is_full(){
    if(buffer[BUFFER_SIZE-1]->id_pezzo == 0)
        return false;

    else{
        printf("Error: buffer is full\n");
        return true;
    }
}

bool is_empty(){
    if(buffer[0]->id_pezzo == 0){
        printf("Error: buffer is empty\n");
        return true;
    }

    else
        return false;
}

bool terminate(){
    for(int i = 0; i < BUFFER_SIZE; i++){
        free(buffer[i]);
    }
    return true;
}