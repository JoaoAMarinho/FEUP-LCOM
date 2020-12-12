#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "i8254.h"
#include "Videocard.h"
#include "KeyBoard.h"
#include "mouse.h"

typedef enum {TIMER, KEYBOARD, MOUSE, RTC} Device;

typedef enum {MAIN, PLAYING, INSTRUCTIONS, BESTSCORES , /*TAREFAS: ALAVANCA, LIGAR FIOS, COLOCAR NÚMEROS ETC*/ PAUSE, TRANSITION, VICTORY, DEFEAT, FINAL} Menu;

//typedef enum {START, TRANSITION, END} Task1;

typedef enum {CAFETERIA, HALLWAY1, ADMIN, WEAPONS, NAVIGATION, HALLWAY2, MEDBAY, ELETRICAL, UPPERENG, LOWERENG, REACTOR, SECURITY, BREAK/*dont know if needed yet*/, END} Room_number;

#include "Player.h"
#include "Menus.h"
#include "Rooms.h"
//#include "Bullet.h"
//#include "DateTime.h"
//#include "MultiPlayer.h"

int gameLoop();

void receiveInterrupt(Device device);

void Play_ih(Device device);

//Tasks interrupt handlers
//void task1IH(Device device);

//Load de cada room
void LoadPlay(Room_number previousRoom);

//Load das tasks
//void loadtaskXPM();

//Load da task conforme o room
//void Loadtask(unsigned int state, Lever * lever);

//Verificar se o player está perto o suficiente da task
//int check_task_position();


//Controlo de todas as task
//bool task1Handler(int * xlen, LeverState * state, struct mouse_ev * mouse_event);
