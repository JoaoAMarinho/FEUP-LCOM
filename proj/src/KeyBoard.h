#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "utils.h"


// Subscribes keyboard
/**
 * @brief Subscribes keyboard interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 *
 * @return int 0 upon success and non-zero otherwise
 */
int(keyboard_subscribe_int)(uint8_t* bit_no);

// Unsubscribes keyboard
/**
 * @brief  Unsubscribes keyboard interrupts
 * 
 * @return int 0 upon success and non-zero otherwise
 */
int (keyboard_unsubscribe_int)();

// Handles keyboard interrupts
/**
 * @brief Handles keyboard interrupts
 * 
 */
void (kbc_ih)();

