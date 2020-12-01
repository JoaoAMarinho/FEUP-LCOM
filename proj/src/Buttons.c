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

/*
void change_background_map(Button * button){
    uint32_t * map, * buttonMap;

    map = (uint32_t *)background_menu.bytes;

    if (!button->mouseOver) {
        buttonMap = (uint32_t *)button->buttonImg.bytes;

        for(int i = 0; i < button->buttonImg.width; i++) {
        for (int j = 0; j < button->buttonImg.height; j++) {
            if (*(buttonMap + i + j*button->buttonImg.width) != xpm_transparency_color(XPM_8_8_8_8))
            *(map + (button->x + i) + (button->y + j) * hres) = *(buttonMap + i + j*button->buttonImg.width);
        }
        }
    }
    else {
        buttonMap = (uint32_t *)button->highlightedImg.bytes;

        for(int i = 0; i < button->highlightedImg.width; i++) {
        for (int j = 0; j < button->highlightedImg.height; j++) {
            if (*(buttonMap + i + j*button->highlightedImg.width) != xpm_transparency_color(XPM_8_8_8_8))
            *(map + (button->x + i) + (button->y + j) * hres) = *(buttonMap + i + j*button->buttonImg.width);
        }
        }
    }
}
*/
