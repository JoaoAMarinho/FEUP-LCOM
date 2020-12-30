#pragma once

#include <lcom/lcf.h>

#include "mouse.h"
#include "Projectile.h"
#include "Buttons.h"
#include "Rtc.h"

//Include das images dos menus
#include "Xpms/Main_Menu/BackGround.xpm"
#include "Xpms/BestScores.xpm"
#include "Xpms/Instructions.xpm"
#include "Xpms/Pause_Menu/PauseBackground.xpm"
#include "Xpms/GameMap.xpm"
#include "Xpms/Victory.xpm"
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

/**
 * @brief enum with all the menus
 * 
 */
typedef enum {MAIN, PLAYING, INSTRUCTIONS, BESTSCORES, ICETASK, SHIPTASK/*TASKS*/, PAUSE, GAMEMAP, VICTORY, DEFEAT, FINAL} Menu;

/**
 * @brief enum with all the possible devices
 * 
 */
typedef enum {TIMER, KEYBOARD, MOUSE, RTC} Device;

/**
 * @struct GameTimer
 *
 * @brief Struct that represents the gameTimer information
 *
 * @var GameTimer::Numbers
 * Array with all the numbers xpms
 */
typedef struct {
    xpm_image_t Numbers[10];

} GameTimer;

/**
 * @struct Score
 *
 * @brief Struct that represents the score information
 *
 * @var Score::points
 * Score points
 * @var Score::data
 * Score date
 */
typedef struct {
    int points;
    Date data;

} Score;

//Menu interrupt handlers
/**
 * @brief Main menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void Main_ih(Device device);

/**
 * @brief BestScores menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void Bestscores_ih(Device device);

/**
 * @brief Instructions menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void Instructions_ih(Device device);

/**
 * @brief Pause menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void Pause_ih(Device device);

/**
 * @brief GameMap menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void GameMap_ih(Device device);

/**
 * @brief Victory menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void Victory_ih(Device device);

/**
 * @brief Defeat menu interrupt handler
 * 
 * @param device which send an interrupt
 */
void Defeat_ih(Device device);

//Tasks interrupt handlers
/**
 * @brief Ice task interrupt handler
 * 
 * @param device which send an interrupt
 */
void Ice_ih(Device device);

/**
 * @brief Ship task interrupt handler
 * 
 * @param device which send an interrupt
 */
void Ship_ih(Device device);

//Load menus
/**
 * @brief Loads the main menu xpm, it's buttons, and draws the menu
 * 
 */
void LoadMain();

/**
 * @brief Loads the bestScores menu xpm, each score, and draws the menu
 * 
 */
void LoadBestsores();

/**
 * @brief Loads the instructions menu xpm and draws it
 * 
 */
void LoadInstructions();

/**
 * @brief Loads the pause menu xpm, it's buttons, and draws the menu
 * 
 */
void LoadPause();

/**
 * @brief Loads the gameMap menu xpm, the task warnings and draws it
 * 
 */
void LoadGameMap();

/**
 * @brief Loads the victory menu xpm, the score and draws it
 * 
 */
void LoadVictory();

/**
 * @brief Loads the defeat menu xpm and draws it
 * 
 */
void LoadDefeat();

/**
 * @brief Loads the numbers xpms to the gameTimer
 * 
 */
void LoadNumbers();

//Load de cada task
/**
 * @brief Draws a specific task
 * 
 * @param index of the task in the gameTasks array
 */
void LoadTask(int index);

//Load de cada room
/**
 * @brief Draws the room as well as all the tasks and opponents from that room, changes the player direction and position according to the room transition 
 * 
 * @param previousRoom the player was located 
 *
 * @param reset bool to know if it is to reset the room
 */
void LoadPlay(Room_number previousRoom,bool reset);

//Draw menus
/**
 * @brief Draws the current_background
 * 
 */
void draw_Menu();


//Game Timer

//Draw countdown time
/**
 * @brief Draws the remaining game time
 * 
 */
void draw_GameTimer();

/**
 * @brief Removes the game time from the screen
 * 
 */
void erase_GameTimer(); //Erase countdown time

/**
 * @brief Draws a specific number in a certain position
 * 
 * @param x position of the number
 *
 * @param y position of the number
 *
 * @param n number to be drawn 
 */
void draw_Number(int x, int y, int n);

//Date
/**
 * @brief Draws the date in the main menu
 * 
 */
void draw_Main_Date();

/**
 * @brief Removes the date from the main menu screen
 * 
 */
void eraseDate();

/**
 * @brief Draws a specific symbol in a certain position
 * 
 * @param x position of the symbol
 *
 * @param y position of the symbol
 *
 * @param n index of the symbol to be drawn 
 */
void draw_Symbol(int x, int y, int n);

//Gesture handlers
/**
 * @brief Task ship mouse gesture handler
 * 
 * @param mouseEvent event occured in the mouse
 *
 * @param reset bool true if it is to reset the handler, false if not
 *
 * @return true if it reached the end state
 * @return false if it did not reach the end state
 */
bool ship_gesture_handler(Mouse_event* mouseEvent,bool reset);

//Scores
/**
 * @brief Tries to insert the current score in the scores array 
 * 
 */
void insertBestScores();

/**
 * @brief Draws the pontos in a certain position
 * 
 * @param x position of the pontos 
 *
 * @param y position of the pontos
 */
void draw_Score(int x, int y);

/**
 * @brief Draws a specific score in a certain position
 * 
 * @param y position of the score
 *
 * @param score to be drawn 
 */
void draw_BestScore(int y, Score score);

/**
 * @brief Draws a specific date in a certain position
 * 
 * @param y position of the date
 *
 * @param date to be drawn 
 */
void draw_Date(int y,Date date);
