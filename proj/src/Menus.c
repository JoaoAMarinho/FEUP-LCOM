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
xpm_image_t empty_background;

static Button ** mainButtons;
//static Button ** pauseButtons;
//static Button * continueButton;

//---------------------------------------------------------------------------------------------
//Interrupt handlers

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
                        erase_button(mainButtons[0]);
                        mainButtons[0]->isMouseOver = false;
                        //add_button(mainButtons[0]);
                        draw_button(mainButtons[0]);
                    }
                    else if (overInstructions) {
                        overInstructions = false;
                        erase_button(mainButtons[1]);
                        mainButtons[1]->isMouseOver = false;
                        add_button(mainButtons[1]);
                        draw_button(mainButtons[1]);
                    }
                    else if (overBestscores) {
                        overBestscores = false;
                        erase_button(mainButtons[2]);
                        mainButtons[2]->isMouseOver = false;
                        add_button(mainButtons[2]);
                        draw_button(mainButtons[2]);
                    }
                    else if (overExit) {
                        overExit = false;
                        erase_button(mainButtons[3]);
                        mainButtons[3]->isMouseOver = false;
                        add_button(mainButtons[3]);
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
                        erase_button(mainButtons[0]);
                        mainButtons[0]->isMouseOver = true;
                        //add_button(mainButtons[0]);
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
                        erase_button(mainButtons[1]);
                        mainButtons[1]->isMouseOver = true;
                        //add_button(mainButtons[1]);
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
                        //add_button(mainButtons[2]);
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
                        //add_button(mainButtons[3]);
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

//---------------------------------------------------------------------------------------------
//Load menus

void LoadMain(){
    xpm_load(MainBackGround, XPM_8_8_8_8, &current_background);
    xpm_load(MainBackGround, XPM_8_8_8_8, &empty_background);

    cursor = create_cursor();

    mainButtons = (Button **) malloc(4 * sizeof(Button *));

    mainButtons[0] = createButton(PLAY_B,570,110);
    mainButtons[1] = createButton(INSTRUCTIONS_B,570,170);
    mainButtons[2] = createButton(BESTSCORES_B,570,230);
    mainButtons[3] = createButton(EXIT_B,570,290);

    
    add_button(mainButtons[0]);
    add_button(mainButtons[1]);
    add_button(mainButtons[2]);
    add_button(mainButtons[3]);
    draw_Main();
}

//---------------------------------------------------------------------------------------------
//Draw menus

void draw_Main() {

    uint32_t* map = (uint32_t*) current_background.bytes;

    for(unsigned int i = 0; i < current_background.width; i++) {
        for (unsigned int j = 0; j < current_background.height; j++) {
            drawPixel(i,j,*(map + i + j*horizontal_res));
        }
    }
}
