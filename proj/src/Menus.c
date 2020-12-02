#include "Menus.h"

extern uint16_t horizontal_res, vertical_res;
extern uint8_t keyboard_data;
extern struct packet mouse_pack;

extern Menu gameMenu;
extern Player * player;
extern Room * room;
extern Cursor * cursor;
//extern Date * date;

xpm_image_t current_background;
//xpm_image_t clean_background_menu;

static Button ** mainButtons;
//static Button ** pauseButtons;
//static Button * continueButton;

//---------------------------------------------------------------------------------------------

void Main_ih(Device device){
    static Mouse_event * mouseEvent;
    bool playClicked = false, instructionsClicked = false, bestscoresClicked = false, exitClicked = false;
    static bool overPlay = false, overInstructions = false, overBestscores = false, overExit = false;

    switch (device) {
        case TIMER:
            switch(checkOverMain()){
                case 'N':
                    if (overPlay) {
                        overPlay = false;
                        mainButtons[0]->isMouseOver = false;
                        //add_button_to_background(mainButtons[0]);
                        draw_button(mainButtons[0]);
                    }
                    else if (overInstructions) {
                        overInstructions = false;
                        mainButtons[1]->isMouseOver = false;
                        //add_button_to_background(mainButtons[1]);
                        draw_button(mainButtons[1]);
                    }
                    else if (overBestscores) {
                        overBestscores = false;
                        mainButtons[2]->isMouseOver = false;
                        //add_button_to_background(mainMenuButtons[2]);
                        draw_button(mainButtons[2]);
                    }
                    else if (overExit) {
                        overExit = false;
                        mainButtons[3]->isMouseOver = false;
                        //add_button_to_background(mainButtons[2]);
                        draw_button(mainButtons[3]);
                    }
                    break;
                case 'P':
                    if ( *mouseEvent == L_UP) {
                        playClicked = true;
                        break;
                    }
                    else if (!overPlay) {
                        overPlay = true;
                        mainButtons[0]->isMouseOver = true;
                        //add_button_to_background(mainButtons[0]);
                        draw_button(mainButtons[0]);
                    }
                    break;
                case 'I':
                    if (*mouseEvent == L_UP){
                        instructionsClicked = true;
                        break;
                    }
                    else if (!overInstructions) {
                        overInstructions = true;
                        mainButtons[1]->isMouseOver = true;
                        //add_button_to_background(mainButtons[1]);
                        draw_button(mainButtons[1]);
                    }
                    break;
                case 'B':
                    if (*mouseEvent == L_UP) {
                        bestscoresClicked = true;
                        break;
                    }
                    else if (!overBestscores) {
                        overBestscores = true;
                        mainButtons[2]->isMouseOver = true;
                        //add_button_to_background(mainButtons[2]);
                        draw_button(mainButtons[2]);
                    }
                    break;
                case 'E': 
                    if (*mouseEvent == L_UP) {
                        exitClicked = true;
                        break;
                    }
                    else if (!overExit) {
                        overExit = true;
                        mainButtons[3]->isMouseOver = true;
                        //add_button_to_background(mainButtons[3]);
                        draw_button(mainButtons[3]);
                    }
                    break;
            }
            break;

        case KEYBOARD: //Nada
            break;

        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor(&mouse_pack);
            break;
        case RTC:
            //clean_clock();
            //updateDateTime();
            //draw_clock();
            break;
    }

    if (exitClicked) // Sair
        gameMenu = FINAL;
    else if (playClicked) { // Entrar no modo Singleplayer
        gameMenu = PLAYING;
        //clean_clock();
        //LoadSinglePLayer(1,true);
    }
    else if (instructionsClicked) { // Entrar no modo Multiplayer
        gameMenu = INSTRUCTIONS;
        //clean_clock();
        //Desenhar instruçoes();
    }
    else if (bestscoresClicked) { // Entrar no Help
        gameMenu = BESTSCORES;
        //desenhar bestscores();
    }
}

void Bestscores_ih(Device device){return;}

void Instructions_ih(Device device){return;}

void Pause_ih(Device device){return;}

void Victory_ih(Device device){return;}

void Defeat_ih(Device device){return;}

void LoadMain(bool firstLoad){
    
}
