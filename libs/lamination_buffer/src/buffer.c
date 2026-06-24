#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

Pezzo **initialize(int buffer_size){
    Pezzo **buffer = malloc(buffer_size * sizeof(Pezzo));
    for(int i = 0; i < buffer_size; i++)
        buffer[i] = NULL;
    return buffer;
}

void new_item(Pezzo *head, Pezzo **buffer, int buffer_size){
    if(is_full(buffer, buffer_size)){
        printf("Errore: questo buffer è pieno\n");
        return;
    }

    else{
        for(int i = 0; i < buffer_size; i++){
            if(buffer[i] == NULL){
                buffer[i] = head;
                break;
            }
        }
    }
}

Pezzo *take_item(Pezzo **buffer, int buffer_size){
    if(is_empty(buffer)){
        printf("Errore: questo buffer è vuoto\n");
        return NULL;
    }

    else{
        Pezzo *in_uscita = buffer[0];
        for(int i = 0; i < buffer_size-1; i++)
            buffer[i] = buffer[i+1];

        buffer[buffer_size-1] = NULL;
        return in_uscita;
    }
}

bool is_full(Pezzo **buffer, int buffer_size){
    if(buffer[buffer_size-1] == NULL)
        return false;

    else
        return true;
}

bool is_empty(Pezzo **buffer){
    if(buffer[0] == NULL)
        return true;

    else
        return false;
}

void terminate(Pezzo **buffer, int buffer_size){
    for(int i = 0; i < buffer_size; i++){
        free(buffer[i]);
    }
    free(buffer);
}
