#include "Projectile.h"

extern Player * player;
extern Room * room;
extern Opponent ** gameOpponents;
extern int n_opponents;

extern uint16_t horizontal_res, vertical_res;

//---------------------------------------------------------------------------------------------

Projectile* create_projectile(Player * player){
    Projectile * projectile = (Projectile *) malloc(sizeof(Projectile));

    projectile->x = player->x;
    projectile->y = player->y;
    projectile->direction = player->direction;
    projectile->projectileSpeed = 15;
    projectile->exists = true;
    
    xpm_image_t img;
    //Load pprojectile animations
	  xpm_load(projectile_xpm, XPM_8_8_8_8, &projectile->projectileImg);
	  projectile->projectileAnimations[0] = projectile->projectileImg;
    xpm_load(projectile_anim1_xpm, XPM_8_8_8_8, &img);
    projectile->projectileAnimations[1] = img;
    xpm_load(projectile_anim2_xpm, XPM_8_8_8_8, &img);
    projectile->projectileAnimations[2] = img;
    xpm_load(projectile_anim3_xpm, XPM_8_8_8_8, &img);
    projectile->projectileAnimations[3] = img;
    free(&img);

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
    if(projectile->direction==UP || projectile->direction==DOWN){
        for(int i = projectile->x; i < projectile->projectileImg.height+projectile->x; ++i){
            for (int j = projectile->y; j < projectile->projectileImg.width+projectile->y; ++j) {
                drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
            }
        }
    }else{
      for (int i = projectile->x; i <= projectile->x + projectile->projectileImg.width; i++) {
        for (int j = projectile->y; j <= projectile->y + projectile->projectileImg.height; j++) {
          if(j<vertical_res && i<horizontal_res)
            drawPixel(i,j,*(backgroundMap + i + j * horizontal_res));
        }
    }
    }
}

bool animate_projectile(Projectile *projectile){
    erase_projectile(projectile);

    switch (projectile->direction) {
        case DOWN:
            if (!projectileCollision(projectile)) {
                projectile->y += projectile->projectileSpeed;
                draw_projectile(projectile);
                return true;
            }
            break;
        case UP:
            if (!projectileCollision(projectile)) {
                projectile->y -= projectile->projectileSpeed;
                draw_projectile(projectile);
                return true;
            }
            break;
        case RIGHT:
            if (!projectileCollision(projectile)) {
                projectile->x += projectile->projectileSpeed;
                draw_projectile(projectile);
                return true;
            }
            break;
        case LEFT:
            if (!projectileCollision(projectile)) {
                projectile->x -= projectile->projectileSpeed;
                draw_projectile(projectile);
                return true;
            }
            break;
        default: 
            break;
    }
    return false;
}

void explode_projectile(Projectile* projectile, int index){
		projectile->projectileImg=projectile->projectileAnimations[index];
		draw_projectile(projectile);
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

bool projectileCollision(Projectile *projectile){
    if(projectile->direction==UP){

      // Collisions with walls
      uint32_t* obstaclesMap=(uint32_t*)room->roomObstacles.bytes;
      for (int i = projectile->x; i <= projectile->x + projectile->projectileImg.width; i++) {
        for (int j = projectile->y; j >= projectile->y - projectile->projectileSpeed; j--) {
            if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8)) {
              projectile->exists = false;
              return true;
            }
        }
      }

      // Collisions with opponents
      bool found=false;
      for (int i = projectile->x; i <= projectile->x + projectile->projectileImg.width; i++) {
        for (int j = projectile->y; j >= projectile->y - projectile->projectileSpeed; j--) {
          for (int k = 0; k < n_opponents; k++)
            if(!gameOpponents[k]->isAlive) continue;
            else if(gameOpponents[k]->opponentRoom==room->currentRoom){
              found=true;
              if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                kill_opponent(k);
                projectile->exists = false;
                return true;
              }
            }
            else if(found) break;
        }
      }
		
    }else if(projectile->direction==DOWN){

      // Collisions with walls
      uint32_t* obstaclesMap=(uint32_t*)room->roomObstacles.bytes;
      for (int i = projectile->x; i <= projectile->x + projectile->projectileImg.width; i++) {
        for (int j = projectile->y + projectile->projectileImg.height; j <= projectile->y + projectile->projectileImg.height + projectile->projectileSpeed; j++) {
            if(j>=vertical_res){
              projectile->exists = false;
              return true;
            }
            if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8)) {
              projectile->exists = false;
              return true;
            }
        }
      }

      // Collisions with opponents
      bool found=false;
      for (int i = projectile->x; i <= projectile->x + projectile->projectileImg.width; i++) {
        for (int j = projectile->y + projectile->projectileImg.height; j <= projectile->y + projectile->projectileImg.height + projectile->projectileSpeed; j++) {
          for (int k = 0; k < n_opponents; k++)
            if(!gameOpponents[k]->isAlive) continue;
            else if(gameOpponents[k]->opponentRoom==room->currentRoom){
              found=true;
              if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                kill_opponent(k);
                projectile->exists = false;
                return true;
              }
            }
            else if(found) break;
        }
      }

    }else if(projectile->direction==LEFT){

      // Collisions with walls
      uint32_t* obstaclesMap=(uint32_t*)room->roomObstacles.bytes;
      for (int i = projectile->x; i >= projectile->x - projectile->projectileSpeed; i--) {
        for (int j = projectile->y; j <= projectile->y + projectile->projectileImg.height; j++) {
            if(i<=0){
              projectile->exists = false;
              return true;
            }
            if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8)) {
              projectile->exists = false;
              return true;
            }
        }
      }

      // Collisions with opponents
      bool found=false;
      for (int i = projectile->x; i >= projectile->x - projectile->projectileSpeed; i--) {
        for (int j = projectile->y; j <= projectile->y + projectile->projectileImg.height; j++) {
          for (int k = 0; k < n_opponents; k++)
            if(!gameOpponents[k]->isAlive) continue;
            else if(gameOpponents[k]->opponentRoom==room->currentRoom){
              found=true;
              if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                kill_opponent(k);
                projectile->exists = false;
                return true;
              }
            }
            else if(found) break;
        }
      }
      
    }else if(projectile->direction==RIGHT){

      // Collisions with walls
      uint32_t* obstaclesMap=(uint32_t*)room->roomObstacles.bytes;
      for (int i = projectile->x + projectile->projectileImg.width; i <= projectile->x + projectile->projectileSpeed + projectile->projectileImg.width; i++) {
        for (int j = projectile->y; j <= projectile->y + projectile->projectileImg.height; j++) {
          if(i>=horizontal_res){
            projectile->exists = false;
            return true;
          }
          if(*(obstaclesMap + i + j * horizontal_res) != xpm_transparency_color(XPM_8_8_8_8)) {
            projectile->exists = false;
            return true;
          }
        }
      }

      // Collisions with opponents
      bool found=false;
      for (int i = projectile->x + projectile->projectileImg.width; i <= projectile->x + projectile->projectileImg.width + projectile->projectileSpeed; i++) {
        for (int j = projectile->y; j <= projectile->y + projectile->projectileImg.height; j++) {
          for (int k = 0; k < n_opponents; k++)
            if(!gameOpponents[k]->isAlive) continue;
            else if(gameOpponents[k]->opponentRoom==room->currentRoom){
              found=true;
              if (i > gameOpponents[k]->x && i < gameOpponents[k]->x + gameOpponents[k]->opponentImg.width && j > gameOpponents[k]->y && j < gameOpponents[k]->y + gameOpponents[k]->opponentImg.height) {
                kill_opponent(k);
                projectile->exists = false;
                return true;
              }
            }
            else if(found) break;
        }
      }
      
    }

    //No collision
    return false;
}

