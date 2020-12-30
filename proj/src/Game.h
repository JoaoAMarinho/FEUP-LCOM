#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "Videocard.h"
#include "i8254.h"
#include "KeyBoard.h"

#include "Menus.h"

/**
 * @brief Game main loop, subscribes and receives all the interrupts from each device, in the end unsubscribes and frees the memory
 * 
 * @return int 0 if game is still rolling, 1 if done
 */
int gameLoop();

/**
 * @brief Redirects the interrupt received to its handler deppending on the current gameMenu 
 * 
 * @param device which send an interrupt
 */
void receiveInterrupt(Device device);

/**
 * @brief Play menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void Play_ih(Device device);

//Verificar se o player está na zona de transição de sala
/**
 * @brief Checks if the player is transiting rooms
 * 
 * @return true if the player is transiting rooms
 * @return false if the player is not transiting rooms
 */
bool roomTransition();

//Reset game conditions
/**
 * @brief Resets the game conditions in order to start from the beginning
 * 
 */
void ResetGame();
