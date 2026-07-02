#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "buffer.h"

void print_list(pezzo *head){
    pezzo *current;
    for(current = head; current != NULL; current = current->next)
        printf("Pezzo ID: %d\n", current->id_pezzo);

    printf("\n");
}

void delete_list(pezzo *head){
    pezzo *current = head;
    pezzo *next;

    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

int main(void){
    pezzo *head = NULL;

    buffer *buffer_a = initialize(6);
    buffer *buffer_b = initialize(7);
    buffer *buffer_c = initialize(11);

    int i = 0;
    while(!is_full(buffer_c)){
        pezzo *nuovo_pezzo = malloc(sizeof(pezzo));

        if(nuovo_pezzo == NULL){
            printf("Errore: memoria non allocata correttamente in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

        i++;
        if(!is_full(buffer_a))
            new_item(nuovo_pezzo, buffer_a);
        else if(!is_full(buffer_b))
            new_item(nuovo_pezzo, buffer_b);
        else if(!is_full(buffer_c))
            new_item(nuovo_pezzo, buffer_c);
    }


    while(!is_empty(buffer_a)){
        pezzo *nuovo_pezzo = NULL;

        if(!is_empty(buffer_c))
            nuovo_pezzo = take_item(buffer_c);
        else if(!is_empty(buffer_b))
            nuovo_pezzo = take_item(buffer_b);
        else if(!is_empty(buffer_a))
            nuovo_pezzo = take_item(buffer_a);

        if (nuovo_pezzo != NULL) {
            printf("Pezzo ID: %d\n", nuovo_pezzo->id_pezzo);
        }
    }

    printf("\nLa lista di partenza era:\n");
    print_list(head);
    delete_list(head);

    terminate(buffer_a);
    terminate(buffer_b);
    terminate(buffer_c);
    return 0;
}