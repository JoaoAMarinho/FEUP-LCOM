#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you

#include "Game.h"
//#include "Enemy.h"
//#include "Lever.h"

//Include dos xpm dos rooms

//#include "Images/ammo.xpm"


typedef struct {
    int posfinalx;
    int posfinaly;
    int lado;
} FinalPos;

typedef struct {
    Room_number currentRoom;

    //xpm_image_t level_background;
    //xpm_image_t level_walls_xpm;

    //uint32_t * level_back;
    //uint32_t * level_walls;

    FinalPos *finalPos;

    //Task ** taskList;
    unsigned int taksNumber;
    
    //Enemy ** enemyList;
    unsigned int enemiesNumber;

} Room;
