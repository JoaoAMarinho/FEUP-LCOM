#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you

#include "Videocard.h"
//#include "Enemy.h"
//#include "Lever.h"

//Include dos xpm dos rooms
#include "Xpms/Rooms/Cafeteria.xpm"
#include "Xpms/Rooms/CafeteriaObstacles.xpm"
#include "Xpms/Rooms/Hallway1.xpm"
#include "Xpms/Rooms/Hallway1Obstacles.xpm"
//#include "Images/ammo.xpm"

typedef enum {CAFETERIA, HALLWAY1, ADMIN, WEAPONS, NAVIGATION, HALLWAY2, MEDBAY, ELETRICAL, UPPERENG, LOWERENG, REACTOR, SECURITY, BREAK/*dont know if needed yet*/, END} Room_number;

typedef struct {
    int posfinalx;
    int posfinaly;
    int lado;
} FinalPos;

typedef struct {
    Room_number currentRoom;
    xpm_image_t roomObstacles;
    xpm_image_t roomBackground;

    //FinalPos *finalPos;

    //Task ** taskList;
    int taksNumber;
    
    //Enemy ** enemyList;
    int enemiesNumber;

} Room;


Room * load_room(Room_number roomNumber);

void delete_room();

void draw_room();

void draw_room_enemies();

void draw_room_tasks();

