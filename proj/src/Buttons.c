#include "Buttons.h"

extern uint16_t horizontal_res, vertical_res;
//extern Room * room;
extern xpm_image_t current_background;

//---------------------------------------------------------------------------------------------

Button * createButton(ButtonInfo buttonInfo, int x, int y){
    Button * button = (Button *) malloc (sizeof(Button));

    button->x = x;
    button->y = y;
    button->buttonInfo = buttonInfo;
    button->isMouseOver = false;

    switch (buttonInfo) {
        case PLAY_B:
            xpm_load(Play_not_clicked, XPM_8_8_8_8, &button->buttonNotClicked);
            xpm_load(Play_clicked, XPM_8_8_8_8, &button->buttonClicked);
            break;
        case BESTSCORES_B:
            xpm_load(BestScores_not_clicked, XPM_8_8_8_8, &button->buttonNotClicked);
            xpm_load(BestScores_clicked, XPM_8_8_8_8, &button->buttonClicked);
            break;
        case INSTRUCTIONS_B:
            xpm_load(Instructions_not_clicked, XPM_8_8_8_8, &button->buttonNotClicked);
            xpm_load(Instructions_clicked, XPM_8_8_8_8, &button->buttonClicked);
            break;
        case EXIT_B:
            xpm_load(Exit_not_clicked, XPM_8_8_8_8, &button->buttonNotClicked);
            xpm_load(Exit_clicked, XPM_8_8_8_8, &button->buttonClicked);
            break;
        //Others
  }

  return button;
}

void draw_button(Button * button){
    uint32_t * map;
    if (button->isMouseOver) {
        map = (uint32_t *)button->buttonClicked.bytes;

        for(int i = 0; i < button->buttonClicked.width; i++) {
            for (int j = 0; j < button->buttonClicked.height; j++) {
                if (*(map + i + j*button->buttonClicked.width) != xpm_transparency_color(XPM_8_8_8_8))
                    drawPixel(button->x+i,button->y+j,*(map + i + j*button->buttonClicked.width));
            }
        }
    }
    else {
        map = (uint32_t *)button->buttonNotClicked.bytes;

        for(int i = 0; i < button->buttonNotClicked.width; i++) {
            for (int j = 0; j < button->buttonNotClicked.height; j++) {
                if (*(map + i + j*button->buttonNotClicked.width) != xpm_transparency_color(XPM_8_8_8_8))
                    drawPixel(button->x+i,button->y+j,*(map + i + j*button->buttonNotClicked.width));
            }
        }
    }
}

void erase_button(Button * button){
    uint32_t* map = (uint32_t*) current_background.bytes;
    
   if (button->isMouseOver) {

        for(int i = button->x; i <= button->x+button->buttonClicked.width; i++) {
            for (int j = button->y; j < button->y + button->buttonClicked.height; j++) {
                if (i < (int)horizontal_res - 1 && j < (int)vertical_res - 1)
                    drawPixel(i,j,*(map  + i + j * horizontal_res));
            }
        }
    }
    else {
        for(int i = button->x; i <= button->x+button->buttonNotClicked.width; i++) {
            for (int j = button->y; j < button->y + button->buttonNotClicked.height; j++) {
                if (i < (int)horizontal_res - 1 && j < (int)vertical_res - 1)
                    drawPixel(i,j,*(map  + i + j * horizontal_res));
            }
        }
    }
}

void add_button(Button * button){
    uint32_t * backgroundMap, * buttonMap;

    backgroundMap = (uint32_t *)current_background.bytes;

    if (button->isMouseOver) {
        buttonMap = (uint32_t *)button->buttonClicked.bytes;

        for(int i = 0; i < button->buttonClicked.width; i++) {
            for (int j = 0; j < button->buttonClicked.height; j++) {
                if (*(buttonMap + i + j*button->buttonClicked.width) != xpm_transparency_color(XPM_8_8_8_8))
                    *(backgroundMap + (button->x + i) + (button->y + j) * horizontal_res) = *(buttonMap + i + j*button->buttonClicked.width);
            }
        }
    }
    else {
        buttonMap = (uint32_t *)button->buttonNotClicked.bytes;

        for(int i = 0; i < button->buttonNotClicked.width; i++) {
            for (int j = 0; j < button->buttonNotClicked.height; j++) {
                if (*(buttonMap + i + j*button->buttonNotClicked.width) != xpm_transparency_color(XPM_8_8_8_8))
                    *(backgroundMap + (button->x + i) + (button->y + j) * horizontal_res) = *(buttonMap + i + j*button->buttonNotClicked.width);
            }
        }    
    }
}

