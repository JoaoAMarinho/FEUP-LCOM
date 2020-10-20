#pragma once

#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"


// Subscribes keyboard
int(keyboard_subscribe_int)(uint8_t* bit_no);

// Unsubscribes keyboard
int (keyboard_unsubscribe_int)();

// Handles keyboard interrupt counter
void (kbc_ih)();

int (keyboard_read_command)();
