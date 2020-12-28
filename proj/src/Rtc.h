#pragma once

#include <lcom/lcf.h>

#include "i8042.h"
//#include "Videocard.c"

//Xpms
#include "Xpms/Numbers/Slash.xpm"

typedef struct {
    unsigned int day,month,year;
    xpm_image_t Symbols[1];
} Date;

int (rtc_subscribe_int)(uint8_t *bit_n);

int (rtc_unsubscribe_int)();

int rtc_enable_update_int(bool enable_disable);

int rtc_read_reg(uint8_t reg);

bool not_valid_rtc();

void LoadRTC();

//Date
Date* create_date();

void updateDate();
