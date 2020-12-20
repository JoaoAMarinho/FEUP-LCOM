#pragma once

#include <lcom/lcf.h>

#include "KeyBoard.h"
#include "Tasks.h"

#include "Xpms/Player/player_right0.xpm"
#include "Xpms/Player/player_right1.xpm"
#include "Xpms/Player/player_right2.xpm"
#include "Xpms/Player/player_right3.xpm"
#include "Xpms/Player/player_right4.xpm"

typedef struct {
    xpm_image_t playerImg;

	int x, y; // current position
	int xspeed, yspeed; // current speed
    bool alive;
    int numberProjectiles;

    xpm_image_t PlayerRight[8];
    Direction direction;

} Player;

Player* create_player();

void draw_player(Player *player);

void erase_player(Player* player);

void change_direction(Player * player, bool * up, bool * down, bool * left, bool * right);

void animate_player(Player *player);

void move_player(Player * player, bool up, bool down, bool left, bool right);

bool room_player_collision(Player* player);

int player_opponent_collision(Player * player);
