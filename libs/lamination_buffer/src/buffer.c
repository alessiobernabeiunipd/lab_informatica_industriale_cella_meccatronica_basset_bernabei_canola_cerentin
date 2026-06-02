#include <stdio.h>
#include <stdbool>

#include "buffer.h"

struct pezzo *buffer[BUFFER_SIZE];

for(i = 0; i<BUFFER_SIZE-1; i++){
    buffer[i].id_pezzo = 0;
    buffer[i].ID_ordine = 0;
    buffer[i].priorità = 0;
}



void new_item(struct pezzo *p){
    if(is_full())
        break; //inserire messaggio di errore

    else{
        for(i = 0; i<BUFFER_SIZE-1; i++){
            if(buffer[i].id_pezzo == 0){
                buffer[i] = p;
                break;
            }
        }
    }
}

struct pezzo *take_item(){
    struct pezzo *in_uscita = buffer[0];
    for(i = 0; i<BUFFER_SIZE-2; i++)
        buffer[i] = buffer[i+1];

    buffer[BUFFER_SIZE-1] = 0
    return in_uscita;
}

bool is_full(){
    if buffer[BUFFER_SIZE-1].id_pezzo == 0
        return false;

    else
        return true;
}
