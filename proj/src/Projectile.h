#pragma once

#include <lcom/lcf.h>
#include "Player.h"
//#include "Enemy.h"
//#include "Menus.h"

#include "Xpms/Projectiles/Projectile.xpm"
#include "Xpms/Projectiles/Projectileanim1.xpm"
#include "Xpms/Projectiles/Projectileanim2.xpm"
#include "Xpms/Projectiles/Projectileanim3.xpm"

typedef struct {

    int x;
    int y;
    xpm_image_t projectileImg;
    xpm_image_t projectileAnimations[3];
    int projectileSpeed;
    Direction direction;
    bool exists;

} Projectile;

Projectile* create_projectile(Player * player);

void draw_projectile(Projectile *projectile);

void erase_projectile(Projectile* projectile);

bool animate_projectile(Projectile *projectile);

Projectile * blast(Player * player);

bool projectileCollision(Projectile *projectile); //Colisões Com paredes ou inimigos

//void enemy_shoot(Enemy * enemy); //Tiro do inimigo

//bool enemyBulletAnimation(Bullet * bullet, Player * player); //Animação da bala do inimigo

//bool check_collision_with_player(Bullet * bullet, Player * player); //Verifica se o inimigo matou o player

