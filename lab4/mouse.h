#pragma once

#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"


// Subscribes mouse
int(mouse_subscribe_int)(uint8_t* bit_n);

// Unsubscribes mouse
int (mouse_unsubscribe_int)();

// Handles mouse interrupt counter
void (mouse_ih)();

void (get_packet)(struct packet *pp);

int (mouse_data_reporting)(uint32_t cmd);


