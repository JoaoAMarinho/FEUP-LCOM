#include "Player.h"

//extern unsigned int hres, vres;
//extern uint8_t kbd_data;


Player* create_player(int x, int y,Direction direction){
    Player * player = (Player *) malloc (sizeof(Player));

    player->x = x;
    player->y = y;
    player->xspeed = 3;
    player->yspeed = 3;
    player->direction=direction;

    xpm_image_t img;

    //Load player animations
    xpm_load(player_right0, XPM_8_8_8, &img);
    player->PlayerRight[0] = img;
    xpm_load(player_right1, XPM_8_8_8, &img);
    player->PlayerRight[1] = img;
    xpm_load(player_right2, XPM_8_8_8, &img);
    player->PlayerRight[2] = img;
    xpm_load(player_right3, XPM_8_8_8, &img);
    player->PlayerRight[3] = img;
    xpm_load(player_right4, XPM_8_8_8, &img);
    player->PlayerRight[4] = img;

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
            if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8))
                drawPixel(player->x+i,player->y+j,*(map + i + j*player->img.width));
            }
        }
        break;
        }

        case UP:
        {
        for(int j = 0 ; j < player->img.height; j++) {
            for (int i = 0 ; i < player->img.width ; i++) {
            if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8))
                drawPixel(player->x + j,player->y + player->img.width - i ,*(map + i + j*player->img.width));
            }
        }
        break;
        }

        case DOWN:
        {
        for(int i = 0; i < player->img.width; i++) {
            for (int j = 0; j < player->img.height; j++) {
            if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8))
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
            if (*(map - i - j*player->img.width) != xpm_transparency_color(XPM_8_8_8))
                drawPixel(player->x+i,player->y+j,*(map - i - j*player->img.width));
            }
        }
        break;
        }
        default:
        break;
    }
}


void destroy_player(Player* player){}

int animate_player(Player *player){return 0;}

int check_Pcollision(Player *player){return 0;}
