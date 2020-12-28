#pragma once

#include <lcom/lcf.h>

#include "mouse.h"
#include "Projectile.h"
#include "Buttons.h"
#include "Rtc.h"

//Include das images dos menus
#include "Xpms/Main_Menu/BackGround.xpm"
//Bestscores
#include "Xpms/Instructions.xpm"
#include "Xpms/Pause_Menu/PauseBackground.xpm"
#include "Xpms/GameMap.xpm"
//Victory
#include "Xpms/Defeat.xpm"

#include "Xpms/Numbers/Number0.xpm"
#include "Xpms/Numbers/Number1.xpm"
#include "Xpms/Numbers/Number2.xpm"
#include "Xpms/Numbers/Number3.xpm"
#include "Xpms/Numbers/Number4.xpm"
#include "Xpms/Numbers/Number5.xpm"
#include "Xpms/Numbers/Number6.xpm"
#include "Xpms/Numbers/Number7.xpm"
#include "Xpms/Numbers/Number8.xpm"
#include "Xpms/Numbers/Number9.xpm"

typedef enum {MAIN, PLAYING, INSTRUCTIONS, BESTSCORES, ICETASK, SHIPTASK/*TASKS: ICETASK, CARDTASK ,ALAVANCA, LIGAR FIOS, COLOCAR NÚMEROS ETC*/, PAUSE, GAMEMAP, VICTORY, DEFEAT, FINAL} Menu;

typedef enum {TIMER, KEYBOARD, MOUSE, RTC} Device;

typedef struct {
    //bool showing;
    xpm_image_t Numbers[10];

} GameTimer;

//Menu interrupt handlers
void Main_ih(Device device);

void Bestscores_ih(Device device);

void Instructions_ih(Device device);

void Pause_ih(Device device);

void GameMap_ih(Device device);

void Victory_ih(Device device);

void Defeat_ih(Device device);

//Tasks interrupt handlers
void Ice_ih(Device device);

void Ship_ih(Device device);
//void CardIH


//Load menus
void LoadMain();

//LoadBestscores();

void LoadInstructions();

void LoadPause();

void LoadGameMap();

//Load victory
void LoadDefeat();

void LoadNumbers();

//Load de cada task
void LoadTask(int index);

//Load de cada room
void LoadPlay(Room_number previousRoom,bool reset);

//Draw menus
void draw_Menu();


//Game Timer
void draw_GameTimer(); //Draw countdown time
void erase_GameTimer(); //Erase countdown time
void draw_Number(int x, int y, int n);

//Date
void draw_Date();
void eraseDate();
void draw_Symbol();

//Gesture handlers
bool ship_gesture_handler(Mouse_event* mouseEvent,bool reset);

