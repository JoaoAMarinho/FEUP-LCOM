#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define BIT(n)    (1 << (n))

#define DELAY_US    20000

#define KBD_IRQ 0x01

#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

#define ESC_KEY    0x81

#define KEYBOARD_OUT_BUF     0x60

#define STATUS_REGISTER   0x64 
#define COMMAND_REGISTER 0x64

#define MAKE_CODE_VERIFICATION BIT(7)

// Status Register State
#define PARITY_ERROR BIT(7)

#define TIMEOUT_ERROR BIT(6)

#define	AUX BIT(5)

#define KEYBOARD_INHIBIT BIT(4)

#define A2_LINE BIT(3)

#define SYSTEM_FLAG BIT(2)

#define INPUT_BUF_FULL BIT(1)

#define OUTPUT_BUF_FULL  BIT(0)

#define TWO_BYTES   0xE0

// Keyboard-Related KBC Commands

#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60
#define CHECK_KBC 0xAA
#define CHECK_KEYBOARD_INTERFACE 0xAB
#define DISABLE_KBD_INTERFACE 0xAD
#define ENABLE_KBD_INTERFACE 0xAE 



#endif /*_LCOM_I8042_H*/
