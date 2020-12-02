#pragma once

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"
#include "Videocard.h"
#include "KeyBoard.h"

#include "Xpms/Cursor.xpm"

typedef struct {
    xpm_image_t img;
    int x, y;
} Cursor;
/*
enum state {
    INITIAL=0,
    MOVE_LEFT,
    SWITCH_SIDE,
    MOVE_RIGHT,
};
*/
typedef enum{
    L_DOWN,
    R_DOWN,
    L_UP,
    R_UP,
    MIDLE,
    MOVE,
} Mouse_event;


// Subscribes mouse
int(mouse_subscribe_int)(uint8_t* bit_n);

// Unsubscribes mouse
int (mouse_unsubscribe_int)();

// Handles mouse interrupt counter
void (mouse_ih)();

void (get_packet)(struct packet *pp);

int (mouse_data_reporting)(uint32_t cmd);

Mouse_event * get_mouse_event(struct packet * mouse_pack);

//Cursor
Cursor * create_cursor();

void update_cursor(struct packet * mouse_pack);

void draw_cursor();

void erase_cursor();

char checkOverMain();

//unsigned int check_collision_pause_menu ();

//unsigned int check_collision_won_menu();


