#pragma once

#include <lcom/lcf.h>

#include "Videocard.h"
#include "KeyBoard.h"
//#include "Rooms.h"

#include "Xpms/Player/player_right0.xpm"
#include "Xpms/Player/player_right1.xpm"
#include "Xpms/Player/player_right2.xpm"
#include "Xpms/Player/player_right3.xpm"
#include "Xpms/Player/player_right4.xpm"

typedef struct {
    xpm_image_t img;

	int x, y; // current position
	int width, height; // dimensions
	int xspeed, yspeed; // current speed
    bool alive;
    int numberBullets;

    xpm_image_t PlayerRight[8];
    Direction direction;

} Player;

Player* create_player(int x, int y,Direction direction);

void draw_player(Player *player);

void erase_player(Player* player);

void change_direction(Player * player, /*uint8_t scancode,*/ bool * up, bool * down, bool * left, bool * right);

void animate_player(Player *player);

void move_player(Player * player, bool up, bool down, bool left, bool right);

bool check_wall_collision(Player * player, Direction direction);

//bool check_enemy_collision(Player * player, Direction direction, Enemy ** enemies, unsigned int numEnemies);
