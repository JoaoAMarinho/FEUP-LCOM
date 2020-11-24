#pragma once

#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "utils.h"

int (set_mode)(uint16_t mode);

void* (vg_init)(uint16_t mode);

int get_vbe_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);

void* (map_mem)(uint16_t mode);

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color);

int (vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len ,uint32_t color);

int drawPixel(uint16_t x,uint16_t y, uint32_t color);

char *get_video_mem();

unsigned get_bytes_per_pixel();

uint16_t getHorizontal();

uint16_t getVertical();

void get_color(uint32_t *color, unsigned row, unsigned column, uint8_t no_rectangles, uint32_t first, uint8_t step);

int drawXpm(uint16_t x,uint16_t y, xpm_image_t *img);
