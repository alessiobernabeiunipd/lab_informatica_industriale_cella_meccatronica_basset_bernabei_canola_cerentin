#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

void print_list(Pezzo *head){
        Pezzo *current = head;

        while(current != NULL){
            printf("ID Pezzo: %d\n", current->id_pezzo);
            current = current->next;
        }
        printf("\n");
    }

void main(){
    Pezzo **buffer_a = initialize();
    Pezzo **buffer_b = initialize();
    Pezzo **buffer_c = initialize();

    Pezzo *head = NULL;
    int i = 0;

    while(!is_full(buffer_c)){
        struct pezzo *nuovo_pezzo;

        nuovo_pezzo = malloc(sizeof(struct pezzo));
        if (nuovo_pezzo == NULL){
            printf("Error: malloc() failed in main() while adding new item\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

        i += 10;

        if(!is_full(buffer_a))
            new_item(head, buffer_a);
        else if(!is_full(buffer_b))
            new_item(head, buffer_b);
        else if(!is_full(buffer_c))
            new_item(head, buffer_c);
    }

    Pezzo *output = NULL;

    while(!is_empty(buffer_a)){
        struct pezzo *nuovo;

        nuovo = malloc(sizeof(struct pezzo));
        if (nuovo == NULL){
            printf("Error: malloc() failed in main() while taking item\n");
            exit(EXIT_FAILURE);
        }   

        if(!is_empty(buffer_c))
            nuovo = take_item(buffer_c);
        else if(!is_empty(buffer_b))
            nuovo = take_item(buffer_b);
        else if(!is_empty(buffer_a))
            nuovo = take_item(buffer_a);
            
        nuovo->next = output;
        output = nuovo;
    }

    printf("I valori in ingresso sono:\n");
    print_list(head);

    printf("I valori in uscita sono:\n");
    print_list(output);

    terminate(buffer_a);
    terminate(buffer_b);
    terminate(buffer_c);
}