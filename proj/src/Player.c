#include "Player.h"

extern uint16_t horizontal_res, vertical_res;
extern uint8_t keyboard_data;
extern Room * room;
extern Opponent ** gameOpponents;
extern int n_opponents;
//extern Menu gameMenu;

Player * player;

//---------------------------------------------------------------------------------------------

Player* create_player(){
    player = (Player *) malloc (sizeof(Player));

    player->x = 420;
    player->y = 240;
    player->direction=DOWN;
    player->xspeed = 3;
    player->yspeed = 3;
    player->numberProjectiles=3;
    player->alive=true;

    xpm_image_t img;

    //Load player animations
    xpm_load(player_right0, XPM_8_8_8_8, &img);
    player->PlayerRight[0] = img;
    xpm_load(player_right1, XPM_8_8_8_8, &img);
    player->PlayerRight[1] = img;
    xpm_load(player_right2, XPM_8_8_8_8, &img);
    player->PlayerRight[2] = img;
    xpm_load(player_right1, XPM_8_8_8_8, &img);
    player->PlayerRight[3] = img;
    xpm_load(player_right0, XPM_8_8_8_8, &img);
    player->PlayerRight[4] = img;
    xpm_load(player_right3, XPM_8_8_8_8, &img);
    player->PlayerRight[5] = img;
    xpm_load(player_right4, XPM_8_8_8_8, &img);
    player->PlayerRight[6] = img;
    xpm_load(player_right3, XPM_8_8_8_8, &img);
    player->PlayerRight[7] = img;

    free(&img);
    player->playerImg = player->PlayerRight[0];

    return player;
}

void draw_player(Player *player){
    uint32_t* map=(uint32_t*)player->playerImg.bytes;

    switch(player->direction) {
        case RIGHT:
        {
        for(int i = 0; i < player->playerImg.width; i++) {
            for (int j = 0; j < player->playerImg.height; j++) {
            if (*(map + i + j*player->playerImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x+i,player->y+j,*(map + i + j*player->playerImg.width));
            }
        }
        break;
        }

        case UP:
        {
        for(int j = 0 ; j < player->playerImg.height; j++) {
            for (int i = 0 ; i < player->playerImg.width ; i++) {
            if (*(map + i + j*player->playerImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x + j,player->y + player->playerImg.width - i ,*(map + i + j*player->playerImg.width));
            }
        }
        break;
        }

        case DOWN:
        {
        for(int i = 0; i < player->playerImg.width; i++) {
            for (int j = 0; j < player->playerImg.height; j++) {
            if (*(map + i + j*player->playerImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x + player->playerImg.height - j,player->y + i,*(map + i + j*player->playerImg.width));
            }
        }
        break;
        }

        case LEFT:
        {
        map += player->playerImg.height * player->playerImg.width;

        for(int i = 0; i < player->playerImg.width; i++) {
            for (int j = 1; j < player->playerImg.height; j++) {
            if (*(map - i - j*player->playerImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x+i,player->y+j,*(map - i - j*player->playerImg.width));
            }
        }
        break;
        }
        default:
        break;
    }
}

void erase_player(Player* player){
    uint32_t* backgroundMap=(uint32_t*)room->roomBackground.bytes;
    if(player->direction==UP || player->direction==DOWN){
        for(int i = player->x; i < player->playerImg.height+player->x; ++i){
            for (int j = player->y; j < player->playerImg.width+player->y; ++j) {
                if(*(backgroundMap + i + j * horizontal_res)!=xpm_transparency_color(XPM_8_8_8_8))
                    drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
            }
        }
    }else{ //Left or right direction
        for(int i = player->x; i < player->playerImg.width+player->x; ++i){
            for (int j = player->y; j < player->playerImg.height+player->y; ++j) {
                if(*(backgroundMap + i + j * horizontal_res)!=xpm_transparency_color(XPM_8_8_8_8))
                    drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
            }
        }
    }
}

void animate_player(Player *player){
    static unsigned int animationNumber = 0;

    erase_player(player);
    draw_player(player);
    player->playerImg = player->PlayerRight[animationNumber];
    animationNumber++;
    if (animationNumber == 7) {
        animationNumber = 0;
    }
}

void change_direction(Player * player, /*uint8_t keyboard_data,*/ bool * up, bool * down, bool * left, bool * right){
    if (keyboard_data == 0x11 /*Make-code W*/ || keyboard_data == 0x48 /*Make-code Up-arrow*/) {
        *up = true;
        //player->direction = UP;
    }
    else if (keyboard_data == 0x1E /*Make-code A*/ || keyboard_data == 0x4B /*Make-code Left-arrow*/) {
        *left = true;
        //player->direction = LEFT;
    }
    else if (keyboard_data == 0x1F /*Make-code S*/ || keyboard_data == 0x50 /*Make-code Down-arrow*/) {
        *down = true;
        //player->direction = DOWN;
    }
    else if (keyboard_data == 0x20 /*Make-code D*/ || keyboard_data == 0x4D /*Make-code Right-arrow*/) {
        *right = true;
        //player->direction = RIGHT;
    }
    else if (keyboard_data == 0x91 /*Break-code W*/ || keyboard_data == 0xC8 /*Break-code Up-arrow*/)
        *up = false;
    else if (keyboard_data == 0x9E /*Break-code A*/ || keyboard_data == 0xCB /*Break-code Left-arrow*/)
        *left = false;  
    else if (keyboard_data == 0x9F /*Break-code S*/ || keyboard_data == 0xD0 /*Break-code Down-arrow*/)
        *down = false;
    else if (keyboard_data == 0xA0 /*Break-code D*/ || keyboard_data == 0xCD /*Break-code Right-arrow*/)
        *right = false;
}

void move_player(Player * player, bool up, bool down, bool left, bool right){ 
    if(!player->alive) return;
    erase_player(player);
    if (up && !right && !left && !down)
        player->direction = UP;
    else if (down && !right && !left && !up) 
        player->direction = DOWN;
    else if (!up && right && !left && !down)
        player->direction = RIGHT;
    else if (!up && !right && left && !down) 
        player->direction = LEFT;

    if (up && !room_player_collision(player)) {
        //erase_player(player);
        if (player->y - player->yspeed < 0)
        player->y = 0;
        else  
        player->y -= player->yspeed;

        if (right && !room_player_collision(player)) {
            if (player->x + player->xspeed + player->playerImg.width > (int)horizontal_res)
                player->x = (int)horizontal_res - player->playerImg.width;
            else  
                player->x += player->xspeed;
        }
        if (left && !room_player_collision(player)) {
            if (player->x - player->xspeed < 0)
                player->x = 0;
            else  
                player->x -= player->xspeed;
        }
        if (down && !room_player_collision(player)) {
            if (player->y + player->yspeed > (int)vertical_res - player->playerImg.height - 4)
                player->y = (int)vertical_res - player->playerImg.height - 4;
            else  
                player->y += player->yspeed;
        } 
        //draw_player(player);
    }
    else if (down && !room_player_collision(player)) {

        //erase_player(player);
        if (player->y + player->yspeed > (int)vertical_res - player->playerImg.height - 4)
            player->y = (int)vertical_res - player->playerImg.height - 4;
        else  
            player->y += player->yspeed;
        
        if (right && !room_player_collision(player)) {
            if (player->x + player->xspeed > (int)horizontal_res - player->playerImg.width)
                player->x = (int)horizontal_res - player->playerImg.width;
            else  
                player->x += player->xspeed;
        }
        if (left && !room_player_collision(player)) {
            if (player->x - player->xspeed < 0)
                player->x = 0;
            else  
                player->x -= player->xspeed;
        }
        //draw_player(player);
    }
    else if (right && !room_player_collision(player)) {
        //erase_player(player);
        if (player->x + player->xspeed + player->playerImg.width > (int)horizontal_res)
            player->x = (int)horizontal_res - player->playerImg.width;
        else  
            player->x += player->xspeed;
        
        if (left && !room_player_collision(player)) {
            if (player->x - player->xspeed < 0)
                player->x = 0;
            else  
                player->x -= player->xspeed;
        }
        //draw_player(player);
    }
    else if (left && !room_player_collision(player)) {
        //erase_player(player);
        if (player->x - player->xspeed < 0)
            player->x = 0;
        else  
            player->x -= player->xspeed;
        //draw_player(player);
    }
    draw_player(player);
}

bool room_player_collision(Player* player){
    uint32_t* obstaclesMap=(uint32_t*)room->roomObstacles.bytes;
    if(player->direction==UP){
        for (int i = player->x + 3; i <= player->x + player->playerImg.width - 3; i++) {
            for (int j = player->y; j >= player->y - player->yspeed; j--) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    else if(player->direction==DOWN){
        for (int i = player->x + 3; i <= player->x + player->playerImg.width - 3; i++) {
            for (int j = player->y + player->playerImg.height; j <= player->y + player->playerImg.height + player->yspeed; j++) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    else if(player->direction==RIGHT){
        for (int i = player->x + player->playerImg.width; i <= player->x + player->xspeed + player->playerImg.width; i++) {
            for (int j = player->y + 3; j <= player->y + player->playerImg.height - 3; j++) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    else if(player->direction==LEFT){
        for (int i = player->x; i >= player->x - player->xspeed; i--) {
            for (int j = player->y + 3; j <= player->y + player->playerImg.height - 3; j++) {
                if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8))
                    return true;
            }
        }
    }
    return false;
}

int opponent_collision(Player * player){
    if(player->direction==UP){
        for (int i = player->x + 3; i <= player->x + player->playerImg.width - 3; i++) {
            for (int j = player->y; j >= player->y - player->yspeed; j--) {
                for(int k=0; k < n_opponents; k++){
                    if(gameOpponents[k]==NULL) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width &&j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return k;
                        }
                    }
                }
            }
        }
    }
    else if(player->direction==DOWN){
        for (int i = player->x + 3; i <= player->x + player->playerImg.width - 3; i++) {
            for (int j = player->y + player->playerImg.height; j <= player->y + player->playerImg.height + player->yspeed; j++) {
                for(int k=0; k < n_opponents; k++){
                    if(gameOpponents[k]==NULL) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return k;
                        }
                    }
                }
            }
        }
    }
    else if(player->direction==RIGHT){
        for (int i = player->x + player->playerImg.width; i <= player->x + player->xspeed + player->playerImg.width; i++) {
            for (int j = player->y + 3; j <= player->y + player->playerImg.height - 3; j++) {
                for(int k=0; k < n_opponents; k++){
                    if(gameOpponents[k]==NULL) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return k;
                        }
                    }
                }
            }
        }
    }
    else if(player->direction==LEFT){
        for (int i = player->x; i >= player->x - player->xspeed; i--) {
            for (int j = player->y + 3; j <= player->y + player->playerImg.height - 3; j++) {
                for(int k=0; k < n_opponents; k++){
                    if(gameOpponents[k]==NULL) continue;
                    else if(gameOpponents[k]->opponentRoom==room->currentRoom){
                        if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                            return k;
                        }
                    }
                }
            }
        }
    }
    return -1;
}
