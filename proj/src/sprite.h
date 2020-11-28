#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include <minix/driver.h>
#include <sys/mman.h>
#include "videocard.h"

typedef struct {
	int x, y; // current position
	int width, height; // dimensions
	int xspeed, yspeed; // current speed
	char *map;
} Sprite;

Sprite* create_sprite(xpm_map_t pic, int x, int y, int xspeed, int yspeed,enum xpm_image_type xpm_type);

void destroy_sprite(Sprite* sp);

int animate_sprite(Sprite *sp);

int draw_sprite(Sprite *sp, char *base);

int check_collision(Sprite *sp, char *base);
