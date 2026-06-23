#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "buffer.h"

    void deleting(struct pezzo *head){
        struct pezzo *current = head;
        struct pezzo *seg;

        while(current != NULL){
            seg = current->next;
            free(current);
            current = seg;
        }
    }

int main(){

    struct pezzo *head = NULL;

    initialize();

    for(int i = 0; i <= BUFFER_SIZE; i++){
        struct pezzo *nuovo_pezzo;

        nuovo_pezzo = malloc(sizeof(struct pezzo));
        if (nuovo_pezzo == NULL){
            printf("Error: malloc() failed in main()\n");
            exit(EXIT_FAILURE);
        }

        nuovo_pezzo->id_pezzo = i+1;
        new_item(nuovo_pezzo);
        nuovo_pezzo->next = head;
        head = nuovo_pezzo;

    }

    for(int i = 0; i <= BUFFER_SIZE; i++){
        struct pezzo *nuovo = take_item();
        if(nuovo != NULL){
            nuovo->next = head;
            head = nuovo;
        }
        else{
            printf("Error: Il buffer è vuoto, restituito NULL\n");
            free(nuovo);
        }
    }

    int i = 0;
    while(i<16){
        printf("Pezzo ID: %d", head->id_pezzo);
        printf("\n");
        head = head->next;
        i++;
    }

    //deleting(head);

    //terminate();
}