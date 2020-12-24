#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "i8254.h"
#include "KeyBoard.h"

#include "Menus.h"


//#include "DateTime.h"

int gameLoop();

void receiveInterrupt(Device device);

void Play_ih(Device device);

//Tasks interrupt handlers
//void task1IH(Device device);




//Load da task conforme o room
//void Loadtask(unsigned int state, Lever * lever);

//Verificar se o player está na zona de transição de sala
bool roomTransition();
//Verificar se o player está perto o suficiente da task
//int check_task_position();


//Controlo de todas as task
//bool task1Handler(int * xlen, LeverState * state, struct mouse_ev * mouse_event);
