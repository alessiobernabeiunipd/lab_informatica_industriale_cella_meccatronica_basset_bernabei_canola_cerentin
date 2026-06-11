/*libreria per inizializzare e modificare pezzi tramite il passaggio della lista
concatenata*/
#ifndef PEZZO_H
#define PEZZO_H

#include "../../commons/include/utils.h"
//#include "utils.h"

//OPERAZIONS ON THE LIST
//add the first pezzo modifying list head
void add_pezzo( pezzo **list_head, pezzo *to_add);

//add a passed pezzo struct in the back of the list
void add_other_pezzo( pezzo *list_head, pezzo *to_add);     

//add a passed pezzo struct to make it the next pezzo to start production
void add_pezzo_next_in_production(pezzo *list_head, pezzo *to_add);

//return ptr tofirst pezzo in the list with a specific status
pezzo *first_pezzo_with_status(pezzo *list_head, piece_status status);

//OPERATIONS ON A SINGLE PEZZO

//modify status of a single piece passing the piece and the status to apply
void modify_status(pezzo *p, piece_status new_status);

//return ptr to copy of a pezzo, useful to add a pezzo to production 
pezzo *copy_pezzo(pezzo *p);

//resets a pezzo to it's original state, useful after copying it to add it to production
void reset_pezzo(pezzo *p);

//returns the status
piece_status get_status(pezzo *p);


#endif