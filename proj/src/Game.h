#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "Videocard.h"
#include "i8254.h"
#include "KeyBoard.h"

#include "Menus.h"


//#include "DateTime.h"

int gameLoop();

void receiveInterrupt(Device device);

void Play_ih(Device device);

//Verificar se o player está na zona de transição de sala
bool roomTransition();

//Reset game conditions
void ResetGame();
