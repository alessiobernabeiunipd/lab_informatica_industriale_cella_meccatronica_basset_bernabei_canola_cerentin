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
    }

int main(){

    Pezzo *head = NULL;

    initialize();

    for(int i = 0; i <= BUFFER_SIZE; i++){
        struct pezzo *nuovo_pezzo;

        nuovo_pezzo = malloc(sizeof(struct pezzo));
        if (nuovo_pezzo == NULL){
            printf("Error: malloc() failed in main() while adding new item\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i+1;
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

        new_item(nuovo_pezzo);
    }

    Pezzo *output = NULL;

    for(int i = 0; i <= BUFFER_SIZE; i++){
        struct pezzo *nuovo;

        nuovo = malloc(sizeof(struct pezzo));
        if (nuovo == NULL){
            printf("Error: malloc() failed in main() while taking item\n");
            exit(EXIT_FAILURE);
        }   

        nuovo = take_item();
        if(nuovo != NULL){
            nuovo->next = output;
            output = nuovo;
        }
        else{
            printf("Error: buffer is empty, cannot take item\n");
        }
    }

    print_list(head);
    print_list(output);
}