#pragma once

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"
#include "Videocard.h"

#include "Xpms/Cursor.xpm"

/** 
 * @struct Cursor
 *
 * @brief Struct that represents the cursor information 
 * 
 * @var Cursor::img
 * Cursor xpm
 * @var Cursor::x
 * Cursor x position
 * @var Cursor::y
 * Cursor y position
 */
typedef struct {
    xpm_image_t img;
    int x, y;
} Cursor;

/**
 * @brief Enum type with the event occured in the mouse
 * 
 */
typedef enum{
    MOVE=0,
    L_DOWN,
    R_DOWN,
    L_UP,
    R_UP,
    MIDLE,
} Mouse_event;


// Subscribes mouse
/**
 * @brief Subscribes mouse interrupts
 * 
 * @param bit_n address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 *
 * @return int 0 upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t* bit_n);

// Unsubscribes mouse
/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return int 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

// Handles mouse interrupt counter
/**
 * @brief Handles mouse interrupts 
 * 
 */
void (mouse_ih)();

/**
 * @brief Builds the mouse packet with information from the mouse
 * 
 * @param pp packet with the information from the mouse
 */
void (get_packet)(struct packet *pp);

/**
 * @brief Enables or disables stream mode data reporting, by sending the respective cmd to the mouse
 * 
 * @param cmd command that determines if the function enables or disables stream mode data reporting
 *
 * @return int 0 upon success and non-zero otherwise
 */
int (mouse_data_reporting)(uint32_t cmd);

/**
 * @brief Get the mouse event object
 * 
 * @param mouse_pack packet with the information from the mouse
 *
 * @return Mouse_event* with the event from the mouse
 */
Mouse_event * get_mouse_event(struct packet * mouse_pack);

//Cursor
/**
 * @brief Create a cursor object
 * 
 * @return Cursor* object created
 */
Cursor * create_cursor();

/**
 * @brief Changes the cursor position according to it's movement
 * 
 * @param mouse_pack packet with the information from the mouse
 */
void update_cursor(struct packet * mouse_pack);

/**
 * @brief Changes the cursor position according to it's movement without drawing it
 * 
 * @param mouse_pack packet with the information from the mouse
 */
void update_cursor_without_draw(struct packet * mouse_pack);

/**
 * @brief Draws the cursor object on the screen
 * 
 */
void draw_cursor();

/**
 * @brief Substitutes the cursor pixels with the background in order to erase it  
 * 
 */
void erase_cursor();

/**
 * @brief Checks cursor and button collision in the main_menu
 * 
 * @return char with information rellated to the buttons collision
 */
char checkOverMain();

/**
 * @brief Checks cursor and button collision in the pause_menu
 * 
 * @return char with information rellated to the buttons collision
 */
char checkOverPause();

/**
 * @brief Checks cursor and ice collision in the ice_task_menu
 * 
 * @return int with information rellated to the ice collision
 */
int checkOverIce();
