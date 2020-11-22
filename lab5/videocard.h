#pragma once

#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "utils.h"

int (set_mode)(uint16_t mode);

void* (vg_init)(uint16_t mode);

void* (map_mem)(uint16_t mode);

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color);

int (vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len ,uint32_t color);

int drawPixel(uint16_t x,uint16_t y, uint32_t color);

void (paint_all)(uint32_t color);
void *(vg_map)(uint16_t mode);
