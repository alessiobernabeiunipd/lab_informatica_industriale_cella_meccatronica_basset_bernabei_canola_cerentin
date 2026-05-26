/*Take the element from the struct to insert it in the array.
 *Put the element in the first free spot.
 *If there is no space left in the buffer send an error message
 *to the controller
 *
 *Function needed:
 *New item in the buffer
 *Remove item from the buffer
 *Check the buffer state
*/

#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 6

/*Defining the data structure that I expect to get passed when
 *calling the function.
 */

struct part_data{
    int part_ID;
    int quantity;
    int priority;
    int target_completing_time;
    int completing_time;
    int laminating_time;
    int press_time;
    int scan_time;
    part_status status;
    int *next;
}

/*Adding new item to the buffer*/
void new_item(int *part_data);

/*Removing item from the buffer*/
part_data take_item();

/*Checking the buffer state*/
bool is_full(int sim_time);

#endif