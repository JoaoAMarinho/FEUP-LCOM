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
#include "Xpms/Opponent/Opponent_atack0.xpm"
#include "Xpms/Opponent/Opponent_atack1.xpm"
#include "Xpms/Opponent/Opponent_atack2.xpm"
#include "Xpms/Opponent/Opponent_atack3.xpm"

typedef struct {
    Room_number opponentRoom;
    xpm_image_t opponentImg;

    int x, y; // current position
	int xspeed, yspeed; // current speed

    xpm_image_t opponentAnimations[8];
    xpm_image_t opponentAtack[4];
    
    unsigned int animationIndex;
    bool isMoving;

    Direction direction;

} Opponent;


void LoadOpponents();

Opponent* create_opponent(int x, int y, Direction direction, Room_number currentRoom, bool hasMovement);

void draw_opponent(Opponent* opponent);

void erase_opponent(Opponent* opponent);

void draw_current_opponents();

void move_opponent(Opponent* opponent);

void animate_opponent(Opponent* opponent);

void opponent_atack(Opponent* opponent, int index);

void kill_opponent(int index);

bool room_opponent_collision(Opponent* opponent);

bool opponent_opponent_colision(Opponent* opponent);
