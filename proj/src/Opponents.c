#include "Opponents.h"

extern uint16_t horizontal_res, vertical_res;
extern Room * room;

Opponent ** gameOpponents;
int n_opponents=1;

//---------------------------------------------------------------------------------------------

void LoadOpponents(){
    gameOpponents = (Opponent **) malloc(1 * sizeof(Opponent *));

    //Opponents ordered by their room
    gameOpponents[0] = create_opponent(500,110,UP,CAFETERIA);
    //gameOpponents[1] = create_opponent(500,170,DOWN,HALLWAY1);
    //gameOpponents[2] = create_opponent(500,230,LEFT,NAVIGATION);

}

Opponent* create_opponent(int x, int y, Direction direction, Room_number currentRoom){
    Opponent* opponent = (Opponent *) malloc (sizeof(Opponent));

    opponent->x = x;
    opponent->y = y;

    opponent->opponentRoom=currentRoom;
    opponent->direction = direction;
    opponent->xspeed = 4;
    opponent->yspeed = 4;
    //enemy->moveAnimation = 0;  saber a animação usar um static

    //enemy->isMoving = true;
    //enemy->isAttacking = false;

    xpm_image_t img;

    //Load opponent animations
    xpm_load(opponent_anim0_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[0] = img;
    xpm_load(opponent_anim1_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[1] = img;
    xpm_load(opponent_anim2_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[2] = img;
    xpm_load(opponent_anim1_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[3] = img;
    xpm_load(opponent_anim0_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[4] = img;
    xpm_load(opponent_anim3_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[5] = img;
    xpm_load(opponent_anim4_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[6] = img;
    xpm_load(opponent_anim3_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[7] = img;

    free(&img);
    opponent->opponentImg = opponent->opponentAnimations[0];

    return opponent;
}

void draw_opponent(Opponent* opponent){
    uint32_t* map=(uint32_t*)opponent->opponentImg.bytes;

    switch(opponent->direction) {
        case RIGHT:
        {
        for(int i = 0; i < opponent->opponentImg.width; i++) {
            for (int j = 0; j < opponent->opponentImg.height; j++) {
            if (*(map + i + j*opponent->opponentImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(opponent->x+i,opponent->y+j,*(map + i + j*opponent->opponentImg.width));
            }
        }
        break;
        }

        case UP:
        {
        for(int j = 0 ; j < opponent->opponentImg.height; j++) {
            for (int i = 0 ; i < opponent->opponentImg.width ; i++) {
            if (*(map + i + j*opponent->opponentImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(opponent->x + j,opponent->y + opponent->opponentImg.width - i ,*(map + i + j*opponent->opponentImg.width));
            }
        }
        break;
        }

        case DOWN:
        {
        for(int i = 0; i < opponent->opponentImg.width; i++) {
            for (int j = 0; j < opponent->opponentImg.height; j++) {
            if (*(map + i + j*opponent->opponentImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(opponent->x + opponent->opponentImg.height - j,opponent->y + i,*(map + i + j*opponent->opponentImg.width));
            }
        }
        break;
        }

        case LEFT:
        {
        map += opponent->opponentImg.height * opponent->opponentImg.width;

        for(int i = 0; i < opponent->opponentImg.width; i++) {
            for (int j = 1; j < opponent->opponentImg.height; j++) {
            if (*(map - i - j*opponent->opponentImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(opponent->x+i,opponent->y+j,*(map - i - j*opponent->opponentImg.width));
            }
        }
        break;
        }
        default:
        break;
    }
}

void erase_opponent(Opponent* opponent){
    uint32_t* backgroundMap=(uint32_t*)room->roomBackground.bytes;
    if(opponent->direction==UP || opponent->direction==DOWN){
        for(int i = opponent->x; i < opponent->opponentImg.height+opponent->x; ++i){
            for (int j = opponent->y; j < opponent->opponentImg.width+opponent->y; ++j) {
                drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
            }
        }
    }else{ //Left or right direction
        for(int i = opponent->x; i < opponent->opponentImg.width+opponent->x; ++i){
            for (int j = opponent->y; j < opponent->opponentImg.height+opponent->y; ++j) {
                drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
            }
        }
    }
}

void kill_opponent(Opponent* opponent) {
  erase_opponent(opponent);
  if (opponent != NULL){
        free(opponent->opponentImg.bytes);
        free(opponent);
        opponent = NULL; 
    }
}

void draw_current_opponents(){
    bool found=false;
    for (int i = 0; i < n_opponents; i++)
        if(gameOpponents[i]==NULL) continue;
        else if(gameOpponents[i]->opponentRoom==room->currentRoom){draw_opponent(gameOpponents[i]); found=true;}
        else if(found) break;
}
