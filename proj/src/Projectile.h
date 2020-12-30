#pragma once

#include <lcom/lcf.h>
#include "Player.h"

#include "Xpms/Projectiles/Projectile.xpm"
#include "Xpms/Projectiles/Projectileanim1.xpm"
#include "Xpms/Projectiles/Projectileanim2.xpm"
#include "Xpms/Projectiles/Projectileanim3.xpm"

/**
 * @struct Projectile
 *
 * @brief Struct that represents the projectile information  
 *
 * @var Projectile::x
 * Projectile x position
 * @var Projectile::y
 * Projectile y position
 * @var Projectile::projectileImg
 * Projectile current xpm
 * @var Projectile::projectileAnimations
 * Array with the projectile animations xpms
 * @var Projectile::projectileSpeed
 * Speed of the projectile in the x or y axis
 * @var Projectile::direction
 * Projectile direction
 * @var Projectile::exists
 * Bool true if projectile exists, false if not
 */
typedef struct {
    int x;
    int y;
    xpm_image_t projectileImg;
    xpm_image_t projectileAnimations[4];
    int projectileSpeed;
    Direction direction;
    bool exists;

} Projectile;

/**
 * @brief Create a projectile object
 * 
 * @param player which is shooting the projectile
 *
 * @return Projectile* object created
 */
Projectile* create_projectile(Player * player);

/**
 * @brief Draws the projectile parameter on the screen
 * 
 * @param projectile to be drawn
 */
void draw_projectile(Projectile *projectile);

/**
 * @brief Substitutes the projectile pixels with the room background in order to erase it
 * 
 * @param projectile pointer of the object to be remove from the screen
 */
void erase_projectile(Projectile* projectile);

/**
 * @brief Changes the projectile position if it does not collide, else it erases it 
 * 
 * @param projectile that is moving
 *
 * @return true if it did not collide with an obstacle
 * @return false if it collided with an obstacle
 */
bool animate_projectile(Projectile *projectile);

/**
 * @brief Change projectile img to the following in the array of animation
 * 
 * @param projectile to be animated
 *
 * @param index of the img in the array of animations
 */
void explode_projectile(Projectile* projectile, int index);

/**
 * @brief Blasts a projectile from the player
 * 
 * @param player that shoots the projectile 
 *
 * @return Projectile* created
 */
Projectile * blast(Player * player);

//Colisões Com paredes ou inimigos
/**
 * @brief Checks if a certain projectile collided with any object 
 * 
 * @param projectile to be checked
 *
 * @return true if the projectile collided
 * @return false if the projectile did not collide
 */
bool projectileCollision(Projectile *projectile);

