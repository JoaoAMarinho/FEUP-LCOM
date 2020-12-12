#include "Bullet.h"

extern Player * player;
extern Room * room;

//extern GameState gameState;

extern uint16_t horizontal_res, vertical_res;

//---------------------------------------------------------------------------------------------

Projectile* create_projectile(Player * player){
    Projectile * projectile = (Projectile *) malloc(sizeof(Projectile));

    projectile->x = player->x;
    projectile->y = player->y;
    projectile->direction = player->direction;
    projectile->speed = 15;
    
    xpm_image_t img;
    //Load pprojectile animations
    xpm_load(projectile_anim1_xpm, XPM_8_8_8_8, &img);
    projectile->projectileAnimations[0] = img;
    xpm_load(projectile_anim2_xpm, XPM_8_8_8_8, &img);
    projectile->projectileAnimations[1] = img;
    xpm_load(projectile_anim3_xpm, XPM_8_8_8_8, &img);
    projectile->projectileAnimations[2] = img;
    free(&img);
    //bullet->active = true;

    xpm_load(projectile_xpm, XPM_8_8_8_8, &projectile->projectileImg);

    return bullet;
}

void draw_projectile(Projectile *projectile);

void erase_projectile(Projectile* projectile);

void animate_projectile(Projectile *projectile);