#pragma once

#include <lcom/lcf.h>
#include "Videocard.h"
//#include "Enemy.h"
#include "Menu.h"

#include "Xpms/Projectiles/Projectile.xpm"
#include "Xpms/Projectiles/Projectileanim1.xpm"
#include "Xpms/Projectiles/Projectileanim2.xpm"
#include "Xpms/Projectiles/Projectileanim3.xpm"

typedef struct {

    int x;
    int y;
    xpm_image_t projectileImg;
    xpm_image_t projectileAnimations[2];
    int projectileSpeed;
    Direction direction;
    //bool active;

} Bullet;

Projectile* create_projectile(Player * player);

void draw_projectile(Projectile *projectile);

void erase_projectile(Projectile* projectile);

void animate_projectile(Projectile *projectile);

Bullet * blast(Player * player);

//bool enemyBulletAnimation(Bullet * bullet, Player * player);

//bool check_collision_with_player(Bullet * bullet, Player * player);