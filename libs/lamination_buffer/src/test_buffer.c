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

    Pezzo **buffer = initialize();

    int i = 0;
    while(!is_full(buffer)){
        Pezzo *nuovo_pezzo = malloc(sizeof(Pezzo));

        if(nuovo_pezzo == NULL){
            printf("Errore: memoria non allocata correttamente in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

        i++;
        new_item(nuovo_pezzo, buffer);
    }

    Pezzo *output = NULL;

    while(!is_empty(buffer)){
        Pezzo *nuovo_pezzo = malloc(sizeof(Pezzo));

        if(nuovo_pezzo == NULL){
            printf("Errore: memoria non allocata correttamente in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo = take_item(buffer);
        nuovo_pezzo->next = output;
        output = nuovo_pezzo;
    }


    print_list(head);
    print_list(output);

    terminate(buffer);
}