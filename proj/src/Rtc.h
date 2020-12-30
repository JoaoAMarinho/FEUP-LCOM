#pragma once

#include <lcom/lcf.h>

#include "i8042.h"

//Xpms
#include "Xpms/Numbers/Slash.xpm"

/**
 * @struct Date
 *
 * @brief Struct that represents the date information  
 * 
 * @var Date::day
 * Current day
 * @var Date::month
 * Current month
 * @var Date::year
 * Current year
 * @var Date:Symbols
 * Array that saves all the symbols xpms used for the date
 */
typedef struct {
    unsigned int day,month,year;
    xpm_image_t Symbols[1];
} Date;

/**
 * @brief Subscribes rtc interrupts
 * 
 * @param bit_n address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 *
 * @return int 0 upon success and non-zero otherwise
 */
int (rtc_subscribe_int)(uint8_t *bit_n);

/**
 * @brief Unsubscribes rtc interrupts
 * 
 * @return int 0 upon success and non-zero otherwise
 */
int (rtc_unsubscribe_int)();

/**
 * @brief Enables or disables the update of the rtc according to the parameter 
 * 
 * @param enable_disable bool true if it is to enable, false otherwise
 *  
 * @return int 0 upon success and non-zero otherwise
 */
int rtc_enable_update_int(bool enable_disable);

/**
 * @brief Reads a register from the rtc to the rtc_date array
 * 
 * @param reg register to be read
 *
 * @return int 0 upon success and non-zero otherwise
 */
int rtc_read_reg(uint8_t reg);

/**
 * @brief Waits for a valid rtc
 * 
 * @return true if it failed, invalid
 * @return false if it succeded, valid
 */
bool not_valid_rtc();

/**
 * @brief Reads the information from the rtc to the rtc_date array
 * 
 */
void LoadRTC();

//Date
/**
 * @brief Create a date object
 * 
 * @return Date* object created
 */
Date* create_date();

/**
 * @brief Updates the date object with the values from the rtc_date array
 * 
 */
void updateDate();
