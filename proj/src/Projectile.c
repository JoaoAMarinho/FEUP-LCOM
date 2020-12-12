#include "Projectile.h"

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
    projectile->projectileSpeed = 15;
    
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

    return projectile;
}

void draw_projectile(Projectile *projectile){
    //Desenhar para cada lado
    uint32_t* projectileMap = (uint32_t*) projectile->projectileImg.bytes;

    switch(projectile->direction) {
        case RIGHT:
            for(int i = 0; i < projectile->projectileImg.width; i++) {
                for (int j = 0; j < projectile->projectileImg.height; j++) {
                    if (*(projectileMap + i + j*projectile->projectileImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                        drawPixel(projectile->x+i,projectile->y+j,*(projectileMap + i + j*projectile->projectileImg.width));
                }
            }
            break;
        case UP:
            for(int j = 0 ; j < projectile->projectileImg.height; j++) {
                for (int i = 0 ; i < projectile->projectileImg.width ; i++) {
                    if (*(projectileMap + i + j*projectile->projectileImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                        drawPixel(projectile->x + j,projectile->y + projectile->projectileImg.width - i ,*(projectileMap + i + j*projectile->projectileImg.width));
                }
            }
            break;
        case DOWN:    
            for(int i = 0; i < projectile->projectileImg.width; i++) {
                for (int j = 0; j < projectile->projectileImg.height; j++) {
                    if (*(projectileMap + i + j*projectile->projectileImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                        drawPixel(projectile->x + projectile->projectileImg.height - j,projectile->y + i,*(projectileMap + i + j*projectile->projectileImg.width));
                }
            }
            break;
        case LEFT:    
            projectileMap += projectile->projectileImg.height * projectile->projectileImg.width;
            for(int i = 0; i < projectile->projectileImg.width; i++) {
                for (int j = 0; j < projectile->projectileImg.height; j++) {
                    if (*(projectileMap - i - j*projectile->projectileImg.width) != xpm_transparency_color(XPM_8_8_8_8))
                        drawPixel(projectile->x+i,projectile->y+j,*(projectileMap - i - j*projectile->projectileImg.width));
                }
            }
            break;
        default:
            break;
    }
}

void erase_projectile(Projectile* projectile){
    uint32_t* backgroundMap=(uint32_t*)room->roomBackground.bytes;
    for (int i = projectile->x; i <= projectile->x + projectile->projectileImg.width; i++) {
        for (int j = projectile->y; j <= projectile->y + projectile->projectileImg.height; j++) {
            drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
        }
    }
}

bool animate_projectile(Projectile *projectile /*,Enemy ** enemies, int enemyNum*/);{
    erase_projectile(projectile);

    switch (projectile->direction) {
        case DOWN:
            if (projectileCollision(projectile)) {
                projectile->y += projectile->speed;
                draw_projectile(projectile);
                return true;
            }
            break;
        case UP:
            if (projectileCollision(projectile)) {
                projectile->y -= projectile->speed;
                draw_projectile(projectile);
                return true;
            }
            break;
        case RIGHT:
            if (projectileCollision(projectile)) {
                projectile->x += projectile->speed;
                draw_projectile(projectile);
                return true;
            }
            break;
        case LEFT:
            if (projectileCollision(projectile)) {
                projectile->x -= projectile->speed;
                draw_projectile(projectile);
                return true;
            }
            break;
        default: 
            break;
    }

    draw_projectile(projectile);
    return false;
}

Projectile * blast(Player * player){
    Projectile * projectile = create_projectile(player);

    if(projectile->direction==UP){
        projectile->x += 43;
        projectile->y -= 18;
    }else if(projectile->direction==LEFT){
        projectile->x -= 19;
        projectile->y += 14;
    }else if(projectile->direction==RIGHT){
        projectile->x += 77;
        projectile->y += 43;
    }else if(projectile->direction==DOWN){
        projectile->x += 14;
        projectile->y += 77;
    }

    return projectile;
}

//Ajeitar função
bool projectileCollision(Projectile *projectile){
    if(projectile->direction==UP){
        // CHECK COLLISIONS WITH ENEMIES
      for (int i = bullet->x; i <= (int)bullet->x + (int)bullet->img.width; i++) {
        for (int j = bullet->y; j >= (int)bullet->y - (int)bullet->speed; j--) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  enemy_death(enemies[k]);
                  bullet->active = false;
                  return true;
            }
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = bullet->x; i <= bullet->x + bullet->img.width; i++) {
        for (unsigned int j = bullet->y; j >= bullet->y - bullet->speed; j--) {
          if(*(level->level_walls + i + j * hres) == 0) {
            bullet->y = j + 1;
            bullet->active = false;
            return true;
          }
        }
      }
    }else if(projectile->direction==DOWN){
        // CHECK COLLISIONS WITH ENEMIES
      for (int i = bullet->x; i <= (int)bullet->x + (int)bullet->img.width; i++) {
        for (int j = bullet->y + bullet->img.height; j <= (int)bullet->y + (int)bullet->img.height + (int)bullet->speed; j++) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  enemy_death(enemies[k]);
                  bullet->active = false;
                  return true;
            }
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = bullet->x; i <= bullet->x + bullet->img.width; i++) {
        for (unsigned int j = bullet->y + bullet->img.height; j <= bullet->y + bullet->img.height + (int)bullet->speed; j++) {
          if(*(level->level_walls + i + j * hres) == 0) {
            bullet->y = j - 1;
            bullet->active = false;
            return true;
          }
        }
      }

    }else if(projectile->direction==LEFT){
        // CHECK COLLISIONS WITH ENEMIES
      for (int i = bullet->x; i >= (int)bullet->x - (int)bullet->speed; i--) {
        for (int j = bullet->y; j <= (int)bullet->y + (int)bullet->img.height; j++) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  enemy_death(enemies[k]);
                  bullet->active = false;
                  return true;
            }
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = bullet->x; i >= bullet->x - bullet->speed; i--) {
        for (unsigned int j = bullet->y; j <= bullet->y + bullet->img.height; j++) {
          if(*(level->level_walls + i + j * hres) == 0) {
            bullet->x = i - 1;
            bullet->active = false;
            return true;
          }
        }
      }
    }else if(projectile->direction==RIGHT){
        // CHECK COLLISIONS WITH ENEMIES
      for (int i = bullet->x + bullet->img.width; i <= (int)bullet->x + (int)bullet->speed + bullet->img.width; i++) {
        for (int j = bullet->y; j <= (int)bullet->y + (int)bullet->img.height; j++) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  enemy_death(enemies[k]);
                  bullet->active = false;
                  return true;
            }
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = bullet->x + bullet->img.width; i <= bullet->x + bullet->speed + bullet->img.width; i++) {
        for (unsigned int j = bullet->y; j <= bullet->y + bullet->img.height; j++) {
          if(*(level->level_walls + i + j * hres) == 0) {
            bullet->x = i + 1;
            bullet->active = false;
            return true;
          }
        }
      }
    }
    //No collision
    return false;
}

//void enemy_shoot(Enemy * enemy); //Tiro do inimigo

//bool enemyBulletAnimation(Bullet * bullet, Player * player); //Animação da bala do inimigo

//bool check_collision_with_player(Bullet * bullet, Player * player); //Verifica se o inimigo matou o player

