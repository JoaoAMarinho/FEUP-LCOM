#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you

#include "Rooms.h"

//Include dos xpm das tasks


typedef enum {T1,T2,T3} Task_type; //Ajeitar dps

typedef struct {
    Room_number taskRoom;
    xpm_image_t taskBackground;

    xpm_image_t taskAnimations[5];
    
    bool taskFinished;

} Task;


//void LoadTasks();
