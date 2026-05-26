#include "buffer.h"

int buffer[BUFFER_SIZE] = {0};
buffer[0] = NULL;


void new_item(int *part_data){
    if(is_full())
        break;

    else{
        for(i = 0; i<BUFFER_SIZE-1; i++){
            if(buffer[i] == NULL && i<=BUFFER_SIZE-2){
                buffer[i+1] = NULL;
                buffer[i] = part_data.part.id;
                break;
            }
            else if(buffer[i] == NULL && i>BUFFER_SIZE-2)
                buffer[i] = part_data.part.id;
        }
    }
}

part_data take_item(){

}

bool is_full(){
    if buffer[BUFFER_SIZE-1] == NULL
        return false;

    else
        return true;
}
