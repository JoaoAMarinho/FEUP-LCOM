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

//Map button 
//Resume button para o menu de pausa
//Leave button para o menu de pausa
//Outros possíveis buttons

typedef enum {PLAY_B, BESTSCORES_B, INSTRUCTIONS_B, EXIT_B, CALENDAR_B} ButtonInfo; //Adicionar os restantes.

typedef struct {
    xpm_image_t buttonNotClicked;
    xpm_image_t buttonClicked;

    int x, y;  //Current position

    bool isMouseOver;  //If mouse is over the Button

    ButtonInfo buttonInfo;

} Button;

Button * createButton(ButtonInfo buttonInfo, int x, int y);

void draw_button(Button * button);

void erase_button(Button * button);

void add_button(Button * button);  //Colocar o botão no current background
