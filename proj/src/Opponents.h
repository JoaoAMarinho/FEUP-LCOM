#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "Rooms.h"

//Include dos xpm dos Opponents
#include "Xpms/Opponent/Opponent_anim0.xpm"
#include "Xpms/Opponent/Opponent_anim1.xpm"
#include "Xpms/Opponent/Opponent_anim2.xpm"
#include "Xpms/Opponent/Opponent_anim3.xpm"
#include "Xpms/Opponent/Opponent_anim4.xpm"

typedef struct {
    Room_number opponentRoom;
    xpm_image_t opponentImg;

    int x, y; // current position
	int xspeed, yspeed; // current speed

    xpm_image_t opponentAnimations[8];

    Direction direction;

} Opponent;


void LoadOpponents();

Opponent* create_opponent(int x, int y, Direction direction, Room_number currentRoom);

void draw_opponent(Opponent* opponent);

void erase_opponent(Opponent* opponent);

void kill_opponent(Opponent* opponent);

void draw_current_opponents();
