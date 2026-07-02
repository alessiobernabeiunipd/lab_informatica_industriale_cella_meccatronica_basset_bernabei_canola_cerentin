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

int main(void){
    pezzo *head = NULL;

    buffer *buf = initialize(5);

    int i = 0;
    while(!is_full(buf)){
        pezzo *nuovo_pezzo = malloc(sizeof(pezzo));

        if(nuovo_pezzo == NULL){
            printf("Errore: memoria non allocata correttamente in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

        i++;
        new_item(nuovo_pezzo, buf);
    }

    pezzo *output = NULL;

    while(!is_empty(buf)){
        pezzo *nuovo_pezzo = take_item(buf);
        nuovo_pezzo->next = output;
        output = nuovo_pezzo;
    }


    print_list(head);
    print_list(output);

    // Dealloca la lista di pezzi generata nel test
    pezzo *curr = head;
    while(curr != NULL){
        pezzo *next = curr->next;
        free(curr);
        curr = next;
    }

    terminate(buf);
    return 0;
}