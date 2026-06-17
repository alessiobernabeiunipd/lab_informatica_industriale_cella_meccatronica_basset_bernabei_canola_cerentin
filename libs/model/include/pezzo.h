/*libreria per inizializzare e modificare pezzi e lista tramite il passaggio della lista
concatenata o del singolo pezzo*/
#ifndef PEZZO_H
#define PEZZO_H

#include "../../commons/include/utils.h"
#include <stdlib.h>

//OPERAZIONS ON THE LIST

//add the first pezzo modifying list head
void add_pezzo( pezzo **list_head, pezzo *to_add);

//add a passed pezzo struct in the back of the list
void add_other_pezzo( pezzo *list_head, pezzo *to_add);     

/*add a passed pezzo struct to make it the next pezzo to start production. Returns null if operation was interrupter for malloc issues. 
If no piece with WAITING_INPUT status is found the piece is added at the end of the list*/
void add_pezzo_next_in_production(pezzo *list_head, pezzo *to_add);

//return ptr to first pezzo in the list with a specific status. Returns NULL is no piece has the requested status
pezzo *first_pezzo_with_status(pezzo *list_head, piece_status status);

//return ptr to last pezzo in the list with a specific status. Returns NULL is no piece has the requested status
pezzo *last_pezzo_with_status(pezzo *list_head, piece_status status);


//OPERATIONS ON A SINGLE PEZZO

//allcates memory for a pezzo and returns NULL if allocation failed. All int fields are initialized at 0, status is CREATED and next points to NULL
pezzo *new_pezzo();

//modify status of a single piece passing the piece and the status to apply
void modify_status(pezzo *p, piece_status new_status);

//return ptr to copy of a pezzo, useful to add a pezzo to production. the copied pezzo returned is a new indipendent allocation.
pezzo *copy_pezzo(pezzo *p);

//resets a pezzo counters and status to it's original state and next to NULL, useful after copying it to add it to production
void reset_pezzo(pezzo *p);

//returns the status of the passed pezzo
piece_status get_status(pezzo *p);

/*updates nominal times with catalogue values, given the pointer to the piece and to the catalogue array. id_pezzo field needs to be initialized.
Returns without doing anithing if the pointer to valori_nominali is NULL*/
void set_nominal_times(pezzo *p, valori_nominali *v);


#endif
