#pragma once

#include <lcom/lcf.h>

#include "mouse.h"
#include "Projectile.h"
#include "Buttons.h"
//#include "rtc.h"
//#include "Lever.h"
//#include "Date.h"

//Include das images dos menus
#include "Xpms/Main_Menu/BackGround.xpm"

typedef enum {MAIN, PLAYING, INSTRUCTIONS, BESTSCORES , /*TAREFAS: ALAVANCA, LIGAR FIOS, COLOCAR NÚMEROS ETC*/ PAUSE, GAMEMAP, VICTORY, DEFEAT, FINAL} Menu;

typedef enum {TIMER, KEYBOARD, MOUSE, RTC} Device;

//Interrupt handlers
void Main_ih(Device device);

void Bestscores_ih(Device device);

void Instructions_ih(Device device);

void Pause_ih(Device device);

void Victory_ih(Device device);

void Defeat_ih(Device device);

//Load menus
void LoadMain();

//Load de cada room
void LoadPlay(Room_number previousRoom);

//Draw menus
void draw_Main();
