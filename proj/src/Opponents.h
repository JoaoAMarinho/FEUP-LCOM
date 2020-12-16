#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you

#include "Rooms.h"

//Include dos xpm dos Opponents

typedef struct {
    Room_number opponentRoom;
    xpm_image_t opponentImg;

    int x, y; // current position
	int width, height; // dimensions //NOT NEEDED
	int xspeed, yspeed; // current speed
    bool alive;

    xpm_image_t opponentAnimations[5];

    Direction direction;

} Opponent;
