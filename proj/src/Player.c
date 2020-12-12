#include "Player.h"

extern uint16_t horizontal_res, vertical_res;
extern uint8_t keyboard_data;
extern Room * room;
//extern Menu gameMenu;

Player * player;

//---------------------------------------------------------------------------------------------

Player* create_player(){
    player = (Player *) malloc (sizeof(Player));

    player->x = 0;
    player->y = 0;
    player->xspeed = 3;
    player->yspeed = 3;
    player->numberBullets=3;
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
    player->img = player->PlayerRight[0];

    return player;
}

void draw_player(Player *player){
    uint32_t* map=(uint32_t*)player->img.bytes;

    switch(player->direction) {
        case RIGHT:
        {
        for(int i = 0; i < player->img.width; i++) {
            for (int j = 0; j < player->img.height; j++) {
            if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x+i,player->y+j,*(map + i + j*player->img.width));
            }
        }
        break;
        }

        case UP:
        {
        for(int j = 0 ; j < player->img.height; j++) {
            for (int i = 0 ; i < player->img.width ; i++) {
            if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x + j,player->y + player->img.width - i ,*(map + i + j*player->img.width));
            }
        }
        break;
        }

        case DOWN:
        {
        for(int i = 0; i < player->img.width; i++) {
            for (int j = 0; j < player->img.height; j++) {
            if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x + player->img.height - j,player->y + i,*(map + i + j*player->img.width));
            }
        }
        break;
        }

        case LEFT:
        {
        map += player->img.height * player->img.width;

        for(int i = 0; i < player->img.width; i++) {
            for (int j = 0; j < player->img.height; j++) {
            if (*(map - i - j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(player->x+i,player->y+j,*(map - i - j*player->img.width));
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
    for(int i = player->x; i < player->img.width+player->x; ++i){
        for (int j = player->y; j < player->img.height+player->y; ++j) {
            drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
        }
    }
}

void animate_player(Player *player){
    static unsigned int animationNumber = 0;

    erase_player(player);
    draw_player(player);
    player->img = player->PlayerRight[animationNumber];
    animationNumber++;
    if (animationNumber == 7) {
        animationNumber = 0;
    }
}

void change_direction(Player * player, /*uint8_t keyboard_data,*/ bool * up, bool * down, bool * left, bool * right){
    if (keyboard_data == 0x11 /*Make-code W*/ || keyboard_data == 0x48 /*Make-code Up-arrow*/) {
        *up = true;
        player->direction = UP;
    }
    else if (keyboard_data == 0x1E /*Make-code A*/ || keyboard_data == 0x4B /*Make-code Left-arrow*/) {
        *left = true;
        player->direction = LEFT;
    }
    else if (keyboard_data == 0x1F /*Make-code S*/ || keyboard_data == 0x50 /*Make-code Down-arrow*/) {
        *down = true;
        player->direction = DOWN;
    }
    else if (keyboard_data == 0x20 /*Make-code D*/ || keyboard_data == 0x4D /*Make-code Right-arrow*/) {
        *right = true;
        player->direction = RIGHT;
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

    if (up && !right && !left && !down)
        player->direction = UP;
    else if (down && !right && !left && !up) 
        player->direction = DOWN;
    else if (!up && right && !left && !down)
        player->direction = RIGHT;
    else if (!up && !right && left && !down) 
        player->direction = LEFT;

    if (up && !check_wall_collision(player,UP)) {
        /*if (!player->hasAmmo && check_collision_ammo(player,UP)) {
        player->hasAmmo = true;
        player->isReloading = true;
        player->isIdle = false;
        }

        player->isIdle = false;
        */
        erase_player(player);
        if (player->y - player->yspeed < 0)
        player->y = 0;
        else  
        player->y -= player->yspeed;

        if (right && !check_wall_collision(player,RIGHT)) {
            /*if (!player->hasAmmo && check_collision_ammo(player,RIGHT)) {
                player->hasAmmo = true;
                player->isReloading = true;
                player->isIdle = false;
            }*/
            if (player->x + player->xspeed + player->img.width > (int)horizontal_res)
                player->x = (int)horizontal_res - player->img.width;
            else  
                player->x += player->xspeed;
        }
        if (left && !check_wall_collision(player,LEFT)) {
        /*
        if (!player->hasAmmo && check_collision_ammo(player,LEFT)) {
            player->hasAmmo = true;
            player->isReloading = true;
            player->isIdle = false;
        }*/
            if (player->x - player->xspeed < 0)
                player->x = 0;
            else  
                player->x -= player->xspeed;
        }
        if (down && !check_wall_collision(player,DOWN)) {/*
        if (!player->hasAmmo && check_collision_ammo(player,DOWN)) {
            player->hasAmmo = true;
            player->isReloading = true;
            player->isIdle = false;
        }*/
            if (player->y + player->yspeed > (int)vertical_res - player->img.height - 4)
                player->y = (int)vertical_res - player->img.height - 4;
            else  
                player->y += player->yspeed;
        } 
        draw_player(player);
    }
    else if (down && !check_wall_collision(player,DOWN)) {

        erase_player(player);
        if (player->y + player->yspeed > (int)vertical_res - player->img.height - 4)
            player->y = (int)vertical_res - player->img.height - 4;
        else  
            player->y += player->yspeed;
        
        if (right && !check_wall_collision(player,RIGHT)) {
            if (player->x + player->xspeed > (int)horizontal_res - player->img.width)
                player->x = (int)horizontal_res - player->img.width;
            else  
                player->x += player->xspeed;
        }
        if (left && !check_wall_collision(player,LEFT)) {
            if (player->x - player->xspeed < 0)
                player->x = 0;
            else  
                player->x -= player->xspeed;
        }
        draw_player(player);
    }
    else if (right && !check_wall_collision(player,RIGHT)) {
        erase_player(player);
        if (player->x + player->xspeed + player->img.width > (int)horizontal_res)
            player->x = (int)horizontal_res - player->img.width;
        else  
            player->x += player->xspeed;
        
        if (left && !check_wall_collision(player,LEFT)) {
            if (player->x - player->xspeed < 0)
                player->x = 0;
            else  
                player->x -= player->xspeed;
        }
        draw_player(player);
    }
    else if (left && !check_wall_collision(player, LEFT)) {
        erase_player(player);
        if (player->x - player->xspeed < 0)
            player->x = 0;
        else  
            player->x -= player->xspeed;
        draw_player(player);
    }
    /*
    if (check_enemy_collision(player, player->direction, level->enemyList, level->numEnemies)) {
        up = false;
        down = false;
        left = false;
        right = false;
    } 
    */
}

bool check_wall_collision(Player * player, Direction direction){return false;}

//bool check_enemy_collision(Player * player, Direction direction, Enemy ** enemies, unsigned int numEnemies){return false;}
