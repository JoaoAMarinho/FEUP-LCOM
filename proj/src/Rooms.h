#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you

#include "Videocard.h"

//Include dos xpm dos rooms
#include "Xpms/Rooms/Cafeteria.xpm"
#include "Xpms/Rooms/CafeteriaObstacles.xpm"
#include "Xpms/Rooms/Hallway1.xpm"
#include "Xpms/Rooms/Hallway1Obstacles.xpm"
#include "Xpms/Rooms/Admin.xpm"
#include "Xpms/Rooms/AdminObstacles.xpm"
#include "Xpms/Rooms/Navigation.xpm"
#include "Xpms/Rooms/NavigationObstacles.xpm"
#include "Xpms/Rooms/Weapons.xpm"
#include "Xpms/Rooms/WeaponsObstacles.xpm"
#include "Xpms/Rooms/Eletrical.xpm"
#include "Xpms/Rooms/EletricalObstacles.xpm"
#include "Xpms/Rooms/Hallway2.xpm"
#include "Xpms/Rooms/Hallway2Obstacles.xpm"
#include "Xpms/Rooms/Hallway3.xpm"
#include "Xpms/Rooms/Hallway3Obstacles.xpm"
#include "Xpms/Rooms/Medbay.xpm"
#include "Xpms/Rooms/MedbayObstacles.xpm"
#include "Xpms/Rooms/Reactor.xpm"
#include "Xpms/Rooms/ReactorObstacles.xpm"
#include "Xpms/Rooms/UpperEngine.xpm"
#include "Xpms/Rooms/UpperEngineObstacles.xpm"
#include "Xpms/Rooms/LowerEngine.xpm"
#include "Xpms/Rooms/LowerEngineObstacles.xpm"
#include "Xpms/Rooms/Security.xpm"
#include "Xpms/Rooms/SecurityObstacles.xpm"

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
