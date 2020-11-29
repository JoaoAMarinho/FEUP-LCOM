#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "utils.h"


// Subscribes keyboard
int(keyboard_subscribe_int)(uint8_t* bit_no);

// Unsubscribes keyboard
int (keyboard_unsubscribe_int)();

// Handles keyboard interrupts
void (kbc_ih)();

void (getEsc)();
