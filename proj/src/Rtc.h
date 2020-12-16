#pragma once

#include <lcom/lcf.h>

#include "i8042.h"

int (rtc_subscribe_int)(uint8_t *bit_n);

int (rtc_unsubscribe_int)();
