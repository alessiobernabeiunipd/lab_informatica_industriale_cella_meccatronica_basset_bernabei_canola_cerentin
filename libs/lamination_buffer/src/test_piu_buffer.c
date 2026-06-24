#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "buffer.h"

void print_list(Pezzo *head){
    Pezzo *current;
    for(current = head; current != NULL; current = current->next)
        printf("Pezzo ID: %d\n", current->id_pezzo);

    printf("\n");
}

void main(){
    Pezzo *head = NULL;

    Pezzo **buffer_a = initialize(4);
    Pezzo **buffer_b = initialize(7);
    Pezzo **buffer_c = initialize(2);

    int i = 0;
    while(!is_full(buffer_c, 2)){
        Pezzo *nuovo_pezzo = malloc(sizeof(Pezzo));

        if(nuovo_pezzo == NULL){
            printf("Errore: memoria non allocata correttamente in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

        i++;
        if(!is_full(buffer_a, 4))
            new_item(nuovo_pezzo, buffer_a, 4);
        else if(!is_full(buffer_b, 7))
            new_item(nuovo_pezzo, buffer_b, 7);
        else if(!is_full(buffer_c, 2))
            new_item(nuovo_pezzo, buffer_c, 2);
    }


    while(!is_empty(buffer_a)){
        Pezzo *nuovo_pezzo;

        if(!is_empty(buffer_c))
            nuovo_pezzo = take_item(buffer_c, 2);
        else if(!is_empty(buffer_b))
            nuovo_pezzo = take_item(buffer_b, 7);
        else if(!is_empty(buffer_a))
            nuovo_pezzo = take_item(buffer_a, 4);

        printf("Pezzo ID: %d\n", nuovo_pezzo->id_pezzo);
    }

    printf("\nLa lista di partenza era:\n");
    print_list(head);

    terminate(buffer_a, 4);
    terminate(buffer_b, 7);
    terminate(buffer_c, 2);
}