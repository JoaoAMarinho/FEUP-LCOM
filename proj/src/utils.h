#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Get the least significant byte from val
 * 
 * @param val value with the bytes information
 *
 * @param lsb pointer that saves the least significant byte
 *
 * @return int 0 on success, non zero otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Get the most significant byte from val
 * 
 * @param val value with the bytes information
 *
 * @param lsb pointer that saves the most significant byte
 *
 * @return int 0 on success, non zero otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Function able read from registers
 *
 * @param port from where the information will be read
 *
 * @param value where the information will be stored
 *
 * @return int 0 on success, non zero otherwise 
 */
int (util_sys_inb)(int port, uint8_t *value);
