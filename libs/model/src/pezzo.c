#include <stdlib.h>
#include "../include/pezzo.h"

void add_pezzo( pezzo **list_head, pezzo *to_add){
    to_add->next = NULL;
    if(*list_head == NULL){
        *list_head = to_add;
        return;
    }
    add_other_pezzo(*list_head, to_add);
}

void add_other_pezzo( pezzo *list_head, pezzo *to_add){
    while(list_head->next != NULL){
        list_head = list_head->next;
    }
    list_head->next = to_add;
}