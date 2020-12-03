#pragma once

#include <lcom/lcf.h>

#include "i8254.h"
#include "Videocard.h"
#include "mouse.h"
#include "Game.h"
#include "Buttons.h"
//#include "rtc.h"
//#include "Lever.h"
//#include "Date.h"

//Include das images dos menus
#include "Xpms/Main_Menu/BackGround.xpm"

//Interrupt handlers
void Main_ih(Device device);

void Bestscores_ih(Device device);

void Instructions_ih(Device device);

void Pause_ih(Device device);

void Victory_ih(Device device);

void Defeat_ih(Device device);

//Load menus
void LoadMain();

//Draw menus
void draw_Main();
