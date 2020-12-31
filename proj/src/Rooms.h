#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you

#include "Videocard.h"

/**
 * @brief enum with all the possible rooms
 * 
 */
typedef enum {CAFETERIA, HALLWAY1, ADMIN, WEAPONS, NAVIGATION, HALLWAY2, MEDBAY, ELETRICAL, UPPERENG, HALLWAY3, LOWERENG, REACTOR, SECURITY, END} Room_number;

/**
 * @struct Room  
 *
 * @brief Struct that represents the room information
 *
 * @var Room::currentRoom
 * Room type
 * @var Room::roomObstacles
 * Image with the obstacles in the room
 * @var Room::roomBackground
 * Image with the room background
 */
typedef struct {
    Room_number currentRoom;
    xpm_image_t roomObstacles;
    xpm_image_t roomBackground;

} Room;

/**
 * @brief Allocates memory for the room and loads its obstacles and background xpms
 * 
 * @param roomNumber specifies the room type
 *
 * @return Room* pointer to the object created
 */
Room * load_room(Room_number roomNumber);

/**
 * @brief Frees the memory allocated for room
 * 
 */
void delete_room();

/**
 * @brief Draws the room background to the screen
 * 
 */
void draw_room();
