#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "Videocard.h"

#include "Xpms/Main_Menu/Buttons/BestScores_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/BestScores_not_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Exit_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Exit_not_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Instructions_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Instructions_not_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Play_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Play_not_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Calendar_clicked.xpm"
#include "Xpms/Main_Menu/Buttons/Calendar_not_clicked.xpm"

#include "Xpms/Pause_Menu/Buttons/Resume_clicked.xpm"
#include "Xpms/Pause_Menu/Buttons/Resume_not_clicked.xpm"
#include "Xpms/Pause_Menu/Buttons/Menu_clicked.xpm"
#include "Xpms/Pause_Menu/Buttons/Menu_not_clicked.xpm"

/**
 * @brief enum with all the button types
 * 
 */
typedef enum {PLAY_B, BESTSCORES_B, INSTRUCTIONS_B, EXIT_B, CALENDAR_B, RESUME_B, MENU_B} ButtonInfo;

/**
 * @struct Button
 *
 * @brief Struct that represents the button information
 *
 * @var Button::buttonNotClicked
 * Image with the xpm of the button not clicked
 * @var Button::buttonClicked
 * Image with the xpm of the button clicked
 * @var Button::x
 * Button x position
 * @var Button::y
 * Button y position
 * @var Button::isMouseOver
 * Bool true if the cursor is over, false otherwise
 * @var Button::buttonInfo
 * Type of the button
 */
typedef struct {
    xpm_image_t buttonNotClicked;
    xpm_image_t buttonClicked;

    int x, y;  //Current position

    bool isMouseOver;  //If mouse is over the Button

    ButtonInfo buttonInfo;

} Button;

/**
 * @brief Create a Button object
 * 
 * @param buttonInfo type of the button created
 *
 * @param x position of the button 
 *
 * @param y position of the button
 *
 * @return Button* object created
 */
Button * createButton(ButtonInfo buttonInfo, int x, int y);

/**
 * @brief Draws the button parameter on the screen
 * 
 * @param button pointer to the object to be drawn
 */
void draw_button(Button * button);

/**
 * @brief Substitutes the button pixels with the menu background in order to erase it
 * 
 * @param button pointer of the object to be remove from the screen
 */
void erase_button(Button * button);

//Colocar o botão no current background
/**
 * @brief Adds the button to the current_background img 
 * 
 * @param button to be added
 */
void add_button(Button * button);
