#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "utils.h"

//Sentido do player ou do enemy
/**
 * @brief Enum type with all the different directions
 * 
 */
typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

/**
 * @brief Changes the videocard mode to the one specified in the paramter
 * 
 * @param mode mode to be set in the videocard
 *
 * @return int 0 on success, non zero otherwise
 */
int (set_mode)(uint16_t mode);

/**
 * @brief Inicializes the videocard memory and changes it's mode
 *
 * @param mode mode to be set in the videocard
 *
 * @return pointer to the allocated memory adress
 */
void* (vg_init)(uint16_t mode);

/**
 * @brief Get the vbe mode information
 * 
 * @param mode specific mode to get the information from
 *  
 * @param vmi_p struct that saves the information from the specific mode
 * 
 * @return int 0 on success, non zero otherwise
 */
int get_vbe_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Allocates memory for the primary and secondary buffer
 * 
 * @return pointer to the allocated memory adress
 */
void* (map_mem)(uint16_t mode);

/**
 * @brief Draws a certain pixel with a specific colour in the secondary buffer
 * 
 * @param x position of the pixel
 * 
 * @param y position of the pixel
 *
 * @param color to be filled in the pixel position  
 *
 * @return int 0 on success, non zero otherwise
 */
int drawPixel(uint16_t x,uint16_t y, uint32_t color);

/**
 * @brief Copies all the pixels from the second buffer to the primary one
 * 
 */
void copy_to_vram();

/**
 * @brief frees the memory allocated for the secondary buffer
 * 
 */
void free_mem();

