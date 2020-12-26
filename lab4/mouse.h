#pragma once

#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"

enum state {
    INITIAL=0,
    MOVE_LEFT,
    SWITCH_SIDE,
    MOVE_RIGHT,
};

enum event {
    L_DOWN=0,
    R_DOWN,
    M_DOWN,
    TWO_DOWN,
    B_UP
};
// Subscribes mouse
int(mouse_subscribe_int)(uint8_t* bit_n);

// Unsubscribes mouse
int (mouse_unsubscribe_int)();

// Handles mouse interrupt counter
void (mouse_ih)();

void (get_packet)(struct packet *pp);

int (mouse_data_reporting)(uint32_t cmd);

int (mouse_polling)();

void (gesture_handler)(struct packet *pp, uint8_t x_len, uint8_t tolerance, enum event m_event, bool *done);

enum event (mouse_get_event)(struct packet *pp);
