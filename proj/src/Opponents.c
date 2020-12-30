#include "Opponents.h"

extern uint16_t horizontal_res, vertical_res;
extern Room * room;

Opponent ** gameOpponents;
int n_opponents=18;

//---------------------------------------------------------------------------------------------

void LoadOpponents(){
    gameOpponents = (Opponent **) malloc(n_opponents * sizeof(Opponent *));

    //Opponents ordered by their room
    gameOpponents[0] = create_opponent(175,425,DOWN,WEAPONS, false);
    gameOpponents[1] = create_opponent(258,510,UP,NAVIGATION, true);
    gameOpponents[2] = create_opponent(477,207,LEFT,HALLWAY1, true);
    gameOpponents[3] = create_opponent(452,515,UP,HALLWAY1, true);
    gameOpponents[4] = create_opponent(324,168,RIGHT,ADMIN,true);
    gameOpponents[5] = create_opponent(639,337,DOWN,ADMIN,true);
    gameOpponents[6] = create_opponent(222,117,DOWN,MEDBAY,true);
    gameOpponents[7] = create_opponent(586,177,LEFT,ELETRICAL,true);
    gameOpponents[8] = create_opponent(296,108,RIGHT,UPPERENG,false);
    gameOpponents[9] = create_opponent(207,280,RIGHT,HALLWAY3,true);
    gameOpponents[10] = create_opponent(572,377,LEFT,HALLWAY3,true);
    gameOpponents[11] = create_opponent(479,222,DOWN,REACTOR,true);
    gameOpponents[12] = create_opponent(296,364,RIGHT,REACTOR,true);
    gameOpponents[13] = create_opponent(339,119,UP,SECURITY,false);
    gameOpponents[14] = create_opponent(284,490,RIGHT,LOWERENG,true);
    gameOpponents[15] = create_opponent(444,513,UP,HALLWAY2,false);
    gameOpponents[16] = create_opponent(157,223,DOWN,HALLWAY2,false);
    gameOpponents[17] = create_opponent(93,352,RIGHT,HALLWAY2,true);

}

Opponent* create_opponent(int x, int y, Direction direction, Room_number currentRoom, bool hasMovement){
    Opponent* opponent = (Opponent *) malloc (sizeof(Opponent));

    opponent->x = x;
    opponent->y = y;

    opponent->opponentRoom=currentRoom;
    opponent->direction = direction;
    opponent->xspeed = 3;
    opponent->yspeed = 3;
    opponent->isMoving=hasMovement;
    opponent->isAlive=true;
    opponent->animationIndex=0;

    xpm_image_t img;

    //Load opponent movement animations
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

    //Load opponent atack animations
    xpm_load(opponent_atack0_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAtack[0] = img;
    xpm_load(opponent_atack1_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAtack[1] = img;
    xpm_load(opponent_atack2_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAtack[2] = img;
    xpm_load(opponent_atack3_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAtack[3] = img;

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

void draw_current_opponents(){
    bool found=false;
    for (int i = 0; i < n_opponents; i++)
        if(!gameOpponents[i]->isAlive) continue;
        else if(gameOpponents[i]->opponentRoom==room->currentRoom){draw_opponent(gameOpponents[i]); found=true;}
        else if(found) break;
}

void move_opponent(Opponent* opponent){
    if(!opponent->isMoving) return;
    erase_opponent(opponent);
    switch(opponent->direction) {
        case LEFT:
            if(!room_opponent_collision(opponent) && !opponent_opponent_colision(opponent)) {
                if (opponent->x - opponent->xspeed < 0) {
                    opponent->direction = RIGHT;
                    opponent->x = 0;
                }
                else{
                    opponent->x -= opponent->xspeed;
                }
            }
            else{
                opponent->direction = RIGHT;
            }
            break;
        case RIGHT:
            if(!room_opponent_collision(opponent)&& !opponent_opponent_colision(opponent)) {
                if (opponent->x + opponent->opponentImg.width + opponent->xspeed > horizontal_res) {
                    opponent-> direction=LEFT;
                    opponent->x=horizontal_res-opponent->opponentImg.width-1;
                }
                else{
                    opponent->x+=opponent->xspeed;
                }
            }
            else {
                opponent->direction = LEFT;
            }
            break;
        case UP:
            if(!room_opponent_collision(opponent)&& !opponent_opponent_colision(opponent)) {
                opponent->y -= opponent->yspeed;
            }else{
                opponent->direction = DOWN;
            }
            break;
        case DOWN:
            if(!room_opponent_collision(opponent)&& !opponent_opponent_colision(opponent)) {
                if (opponent->y + opponent->yspeed > vertical_res - opponent->opponentImg.width) {
                    opponent->direction = UP;
                    opponent->y = vertical_res - opponent->opponentImg.width - 1;
                }
                else{
                    opponent->y += opponent->yspeed;
                }
            }
            else {
                opponent->direction = UP;
            }
            break;
  }

  draw_opponent(opponent);
}

void animate_opponent(Opponent* opponent){
    erase_opponent(opponent);
    draw_opponent(opponent);
    opponent->opponentImg = opponent->opponentAnimations[opponent->animationIndex];
    opponent->animationIndex++;
    opponent->animationIndex=opponent->animationIndex%8;
}

void opponent_atack(Opponent* opponent, int index){
		opponent->opponentImg=opponent->opponentAtack[index];
		draw_opponent(opponent);
}

void kill_opponent(int index) {
  erase_opponent(gameOpponents[index]);
  if (gameOpponents[index]->isAlive){
        gameOpponents[index]->isAlive=false; 
    }
}

bool room_opponent_collision(Opponent* opponent){
    uint32_t* obstaclesMap=(uint32_t*)room->roomObstacles.bytes;
    if(opponent->direction==UP){
        for (int i = opponent->x; i <= opponent->x + opponent->opponentImg.width; i++) {
            for (int j = opponent->y; j >= opponent->y - opponent->yspeed; j--) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    else if(opponent->direction==DOWN){
        for (int i = opponent->x; i <= opponent->x + opponent->opponentImg.width; i++) {
            for (int j = opponent->y + opponent->opponentImg.height; j <= opponent->y + opponent->opponentImg.height + opponent->yspeed; j++) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    else if(opponent->direction==RIGHT){
        for (int i = opponent->x + opponent->opponentImg.width; i <= opponent->x + opponent->xspeed + opponent->opponentImg.width; i++) {
            for (int j = opponent->y; j <= opponent->y + opponent->opponentImg.height; j++) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    else if(opponent->direction==LEFT){
        for (int i = opponent->x; i >= opponent->x - opponent->xspeed; i--) {
            for (int j = opponent->y; j <= opponent->y + opponent->opponentImg.height; j++) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    return false;
}

bool opponent_opponent_colision(Opponent* opponent){
    if(opponent->direction==UP){
        for (int i = opponent->x; i <= opponent->x + opponent->opponentImg.width; i++) {
            for (int j = opponent->y; j >= opponent->y - opponent->yspeed; j--) {
                for(int k=0; k < n_opponents; k++){
                    if(!gameOpponents[k]->isAlive) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom && gameOpponents[k]!=opponent){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width &&j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    else if(opponent->direction==DOWN){
        for (int i = opponent->x; i <= opponent->x + opponent->opponentImg.width; i++) {
            for (int j = opponent->y + opponent->opponentImg.height; j <= opponent->y + opponent->opponentImg.height + opponent->yspeed; j++) {
                for(int k=0; k < n_opponents; k++){
                    if(!gameOpponents[k]->isAlive) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom && gameOpponents[k]!=opponent){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    else if(opponent->direction==RIGHT){
        for (int i = opponent->x + opponent->opponentImg.width; i <= opponent->x + opponent->xspeed + opponent->opponentImg.width; i++) {
            for (int j = opponent->y; j <= opponent->y + opponent->opponentImg.height; j++) {
                for(int k=0; k < n_opponents; k++){
                    if(!gameOpponents[k]->isAlive) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom && gameOpponents[k]!=opponent){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    else if(opponent->direction==LEFT){
        for (int i = opponent->x; i >= opponent->x - opponent->xspeed; i--) {
            for (int j = opponent->y; j <= opponent->y + opponent->opponentImg.height; j++) {
                for(int k=0; k < n_opponents; k++){
                    if(!gameOpponents[k]->isAlive) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom && gameOpponents[k]!=opponent){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
