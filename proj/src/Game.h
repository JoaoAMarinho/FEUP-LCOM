#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "i8254.h"
#include "Videocard.h"
#include "KeyBoard.h"
#include "Player.h"
#include "mouse.h"
#include "timer.h"
//#include "Bullet.h"
//#include "DateTime.h"
//#include "Menu.h"
//#include "MultiPlayer.h"
//#include "Level.h"

enum Device {TIMER=0, KEYBOARD, MOUSE, RTC};

enum Menu{MAIN=0, VICTORY, DEFEAT, /*TAREFAS: ALAVANCA, LIGAR FIOS, COLOCAR NÚMEROS ETC*/ PAUSE, TRANSITION, HELP, FINAL};

//enum Task1{START=0, TRANSITION, END};

enum Room{CAFETERIA=0, ADMIN, WEAPONS, NAVIGATION, MEDBAY, ELETRICAL, UPPERENG, LOWERENG, REACTOR, SECURITY, END};

int gameLoop();

void receiveInterrupt(enum Device device);

void interruptHandler(enum Device device);

//Tasks interrupt handlers
//void task1IH(Device device);

//Load de cada room
//void loadRoom(bool begin);

//Load das tasks
//void loadtaskXPM();

//Load da task conforme o room
//void Loadtask(unsigned int state, Lever * lever);

//Verificar se o player está perto o suficiente da task
//int check_task_position();

/**
 struct mouse_ev * mouse_events(struct packet * pack);
 */

//Controlo de todas as task
//bool task1Handler(int * xlen, LeverState * state, struct mouse_ev * mouse_event);
