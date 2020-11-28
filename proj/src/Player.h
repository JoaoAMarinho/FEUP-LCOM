#pragma once

#include <lcom/lcf.h>

#include "videocard.h"

#include "Xpms/Player_right/player_right0.xpm"
#include "Xpms/Player_right/player_right1.xpm"
#include "Xpms/Player_right/player_right2.xpm"
#include "Xpms/Player_right/player_right3.xpm"
#include "Xpms/Player_right/player_right4.xpm"

typedef struct {
    xpm_image_t img;

	int x, y; // current position
	int width, height; // dimensions
	int xspeed, yspeed; // current speed

    xpm_image_t PlayerRight[5];
    Direction direction;

} Player;

Player* create_player(int x, int y,Direction direction);

void draw_player(Player *player);

void destroy_player(Player* player);

int animate_player(Player *player);

int check_Pcollision(Player *player);
