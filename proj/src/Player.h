#pragma once

#include <lcom/lcf.h>

#include "KeyBoard.h"
#include "Tasks.h"

#include "Xpms/Player/player_right0.xpm"
#include "Xpms/Player/player_right1.xpm"
#include "Xpms/Player/player_right2.xpm"
#include "Xpms/Player/player_right3.xpm"
#include "Xpms/Player/player_right4.xpm"

/**
 * @struct Player
 *
 * @brief Struct that represents the player information
 *
 * @var Player::playerImg
 * Player xpm
 * @var Player::x
 * Player x position
 * @var Player::y
 * Player y position
 * @var Player::xspeed
 * Player speed in the x axis
 * @var Player::yspeed
 * Player speed in the y axis
 * @var Player::alive
 * Bool true if player alive, false if not
 * @var Player::numberProjectiles
 * Number of player projectiles
 * @var Player::PlayerRight
 * Array with all the player animation xpms
 * @var Player::direction
 * Direction of the player
 */
typedef struct {
    xpm_image_t playerImg;

	int x, y; // current position
	int xspeed, yspeed; // current speed
    bool alive;
    int numberProjectiles;

    xpm_image_t PlayerRight[8];
    Direction direction;

} Player;

/**
 * @brief Create a player object
 * 
 * @return Player* object created
 */
Player* create_player();

/**
 * @brief Draws the player parameter on the screen
 * 
 * @param player pointer to the object to be drawn
 */
void draw_player(Player *player);

/**
 * @brief Substitutes the player pixels with the room background in order to erase it
 * 
 * @param player pointer of the object to be remove from the screen
 */
void erase_player(Player* player);

/**
 * @brief Change the boolean pointers according to the keyboard input to know the way the player is moving
 * 
 * @param up true if player is moving up, false if not
 *
 * @param down true if player is moving down, false if not
 *
 * @param left true if player is moving left, false if not
 *
 * @param right true if player is moving right, false if not
 */
void change_direction(bool * up, bool * down, bool * left, bool * right);

/**
 * @brief Change the player image with the following index in the array of animations
 * 
 * @param player to be animated on the screen
 */
void animate_player(Player *player);

/**
 * @brief Changes the player direction and position if it does not collide with any object
 * 
 * @param player to be moved
 *
 * @param up true if player is moving up, false if not
 *
 * @param down true if player is moving down, false if not
 *
 * @param left true if player is moving left, false if not
 *
 * @param right true if player is moving right, false if not
 */
void move_player(Player * player, bool up, bool down, bool left, bool right);

/**
 * @brief Checks the player collision with the room obstacles
 * 
 * @param player to check the collision
 *
 * @return true if collides 
 * @return false if player does not collide
 */
bool room_player_collision(Player* player);

/**
 * @brief Checks the player collision with the room opponents
 * 
 * @param player to check the collision
 *
 * @return int -1 if it does not collid else index of the opponent in the gameOpponents array 
 */
int player_opponent_collision(Player * player);
