#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

void initialize(){
    for(int i = 0; i <= BUFFER_SIZE-1; i++)
        buffer[i] = 0;
}

void new_item(Pezzo *p){
    if(is_full())
        return;

    else{
        for(int i = 0; i <= BUFFER_SIZE-1; i++){
            if(buffer[i] == NULL){
                buffer[i] = p;
                break;
            }
        }
    }
}

Pezzo *take_item(){
    if(is_empty())
        return NULL;
    
    else{
        Pezzo *in_uscita = buffer[0];
        for(int i = 0; i <= BUFFER_SIZE-2; i++)
            buffer[i] = buffer[i+1];

        buffer[BUFFER_SIZE-1] = NULL;
        return in_uscita;
    }
}

bool is_full(){
    if(buffer[BUFFER_SIZE-1] == NULL)
        return false;

    else{
        printf("Error: buffer is full\n");
        return true;
    }
}

bool is_empty(){
    if(buffer[0] == NULL){
        printf("Error: buffer is empty\n");
        return true;
    }

    else
        return false;
}
