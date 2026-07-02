#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

buffer *initialize(int buffer_size){
    buffer *buf = malloc(sizeof(buffer));
    if (buf == NULL) return NULL;
    buf->array = malloc(buffer_size * sizeof(pezzo *));
    if (buf->array == NULL) {
        free(buf);
        return NULL;
    }
    buf->array_size = buffer_size;

    for(int i = 0; i < buffer_size; i++)
        buf->array[i] = NULL;
    return buf;
}

void new_item(pezzo *head, buffer *buf){
    if(is_full(buf)){
        printf("Errore: questo buffer è pieno\n");
        return;
    }

    else{
        for(int i = 0; i < buf->array_size; i++){
            if(buf->array[i] == NULL){
                buf->array[i] = head;
                break;
            }
        }
    }
}

pezzo *take_item(buffer *buf){
    if(is_empty(buf)){
        printf("Errore: questo buffer è vuoto\n");
        return NULL;
    }

    else{
        pezzo *in_uscita = buf->array[0];
        for(int i = 0; i < buf->array_size-1; i++)
            buf->array[i] = buf->array[i+1];

        buf->array[buf->array_size-1] = NULL;
        return in_uscita;
    }
}

bool is_full(buffer *buf){
    if(buf->array[buf->array_size-1] == NULL)
        return false;

    else
        return true;
}

bool is_empty(buffer *buf){
    if(buf->array[0] == NULL)
        return true;

    else
        return false;
}

void terminate(buffer *buf){
    // Nota: gli elementi pezzo* all'interno del buffer fanno parte della lista globale
    // e verranno liberati durante il cleanup della cella. Qui liberiamo solo la struttura del buffer.
    free(buf->array);
    free(buf);
}
