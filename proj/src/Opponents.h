#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "Rooms.h"

/**
 * @struct Opponent
 *
 * @brief Struct that represents the opponent information
 *
 * @var Opponent::opponentRoom
 * Room in which the opponent belongs
 * @var Opponent::opponentImg
 * Opponent xpm
 * @var Opponent::x
 * Opponent x position
 * @var Opponent::y
 * Opponent y position
 * @var Opponent::xspeed
 * Opponent speed in the x axis
 * @var Opponent::yspeed
 * Opponent speed in the y axis
 * @var Opponent::opponentAnimations
 * Array with all the opponent movement animation xpms
 * @var Opponent::opponentAtack
 * Array with all the opponent atack animation xpms
 * @var Opponent::animationIndex
 * unsigned int with the index of the current animation
 * @var Opponent::isMoving
 * true if opponent is moving, false if not
 * @var Opponent::isAlive
 * true if opponent is alive, false if not
 * @var Opponent::direction
 * Opponent direction
 */
typedef struct {
    Room_number opponentRoom;
    xpm_image_t opponentImg;

    int x, y; // current position
	int xspeed, yspeed; // current speed

    xpm_image_t opponentAnimations[8];
    xpm_image_t opponentAtack[4];
    
    unsigned int animationIndex;
    bool isMoving;
    bool isAlive;

    Direction direction;

} Opponent;

/**
 * @brief Allocates memory for all the existing opponents in the gameOpponents array
 * 
 */
void LoadOpponents();

/**
 * @brief Create a opponent object
 * 
 * @param x position of the opponent
 *
 * @param y position of the opponent
 * @param direction of the opponent
 *
 * @param currentRoom of the opponent 
 *
 * @param hasMovement true if opponent moves, false if not 
 *
 * @return Opponent* object created 
 */
Opponent* create_opponent(int x, int y, Direction direction, Room_number currentRoom, bool hasMovement);

/**
 * @brief Draws the opponent parameter on the screen
 * 
 * @param opponent pointer to the object to be drawn
 */
void draw_opponent(Opponent* opponent);

/**
 * @brief Substitutes the opponent pixels with the room background in order to erase it
 * 
 * @param opponent pointer of the object to be remove from the screen
 */
void erase_opponent(Opponent* opponent);

/**
 * @brief Draws all the opponents with the same room as the room variable
 * 
 */
void draw_current_opponents();

/**
 * @brief Changes the player position if it does not collide with any object, else changes direction
 * 
 * @param opponent to be moved
 */
void move_opponent(Opponent* opponent);

/**
 * @brief Change the opponent image with the following index in the array of animations
 * 
 * @param opponent to be animated on the screen
 */
void animate_opponent(Opponent* opponent);

/**
 * @brief Change the opponent image with the following index in the array of atack animations
 * 
 * @param opponent that is atacking
 *
 * @param index of the img in the array of atack animations
 */
void opponent_atack(Opponent* opponent, int index);

/**
 * @brief Changes the boolean value of Opponent::isAlive to false
 * 
 * @param index of the opponent in the array of gameOpponents
 */
void kill_opponent(int index);

/**
 * @brief Checks the collision of an opponent with the room obstacles
 * 
 * @param opponent to be checked 
 *
 * @return true if it collides
 * @return false if it does not collide
 */
bool room_opponent_collision(Opponent* opponent);

/**
 * @brief Checks the collision of an opponent with another from the same room
 * 
 * @param opponent to be checked 
 *
 * @return true if it collides
 * @return false if it does not collide
 */
bool opponent_opponent_colision(Opponent* opponent);
