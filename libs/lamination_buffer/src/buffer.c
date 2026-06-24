#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

Pezzo **initialize(){
    Pezzo **buffer = malloc(BUFFER_SIZE * sizeof(Pezzo *));
    for(int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = NULL;
    return buffer;
}

void new_item(Pezzo *head, Pezzo **buffer){
    if(is_full(buffer)){
        printf("Errore: questo buffer è pieno\n");
        return;
    }

    else{
        for(int i = 0; i < BUFFER_SIZE; i++){
            if(buffer[i] == NULL){
                buffer[i] = head;
                break;
            }
        }
    }
}

Pezzo *take_item(Pezzo **buffer){
    if(is_empty(buffer)){
        printf("Errore: questo buffer è vuoto\n");
        return NULL;
    }

    else{
        Pezzo *in_uscita = buffer[0];
        for(int i = 0; i < BUFFER_SIZE-1; i++)
            buffer[i] = buffer[i+1];

        buffer[BUFFER_SIZE-1] = NULL;
        return in_uscita;
    }
}

bool is_full(Pezzo **buffer){
    if(buffer[BUFFER_SIZE-1] == NULL)
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

void terminate(Pezzo **buffer){
    for(int i = 0; i < BUFFER_SIZE; i++){
        free(buffer[i]);
    }
    free(buffer);
}
