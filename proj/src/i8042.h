#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

//#define BIT(n)    (1 << (n))
//#define PB2BASE(x) (((x) >> 4) & 0x0F000)
//#define PB2OFF(x) ((x) & 0x0FFFF)

#define DELAY 20000

#define MOUSE_IRQ 12
#define KBD_IRQ 0x01
#define RTC_IRQ 0x08

//Some Break codes
#define ESC_KEY 0x81
#define SPACEBAR_KEY 0xB9
#define M_KEY 0xB2
#define E_KEY 0x92
// Keyboard codes

#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

#define MAKE_CODE_VERIFY BIT(7)

#define MOUSE_OUT_BUF 0x60
#define KEYBOARD_OUT_BUF 0x60
#define STATUS_REGISTER 0x64 
#define COMMAND_REGISTER 0x64

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

// Mouse acknowledgments

#define ACK 0xFA   //if everything ok
#define NACK 0xFE  //if byte is invalid
#define ERROR 0xFC //if 2 consecutive bytes are invalid

#define WRITE_MOUSE_BYTE 0xD4 	
#define WRITE_CMD_BYTE   0x60

// Mouse Commands

#define MOUSE_ENABLE 0xF4 	 
#define MOUSE_DISABLE 0xF5 
#define MOUSE_READ_REQ 0xEB //Send data packet request
#define MOUSE_STREAM 0xEA //Enable Stream Mode

//Mouse Buttons

#define Y_OVFL        BIT(7)    
#define X_OVFL        BIT(6)    
#define Y_SIGN        BIT(5)    
#define X_SIGN        BIT(4)    
#define MIDDLE_BUTTON BIT(2)    
#define RIGHT_BUTTON  BIT(1)    
#define LEFT_BUTTON   BIT(0) 

//Videocard service

#define VIDEO_CARD 0x10
#define PC_CONFIG 0x11
#define MEMORY_CONFIG 0x12
#define KEYBOARD_SERVICE 0x16

//Grafics mode

#define MODE1 0x105 //1024x768, bits per pixel: 8
#define MODE2 0x110 //640x480, bits per pixel: 15(5:5:5)
#define MODE3 0x115 //800x600, bits per pixel: 24(8:8:8)
#define MODE4 0x11A //1280x1024, bits per pixel: 16(5:6:5)
#define MODE5 0x14C //1152x864, bits per pixel: 32((8:)8:8:8)

//RTC defines
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

//#define RTC_SEC  0x00
//#define RTC_MIN  0x02
//#define RTC_HOUR    0x04
//#define RTC_DAY_OF_WEEK 0x06
#define RTC_D   0x07
#define RTC_M   0x08
#define RTC_Y   0x09

//Register A
#define RTC_A_REGISTER  0x0A
#define RTC_UIP BIT(7)

//Register B
#define RTC_B_REGISTER 0x0B
#define RTC_SET BIT(7)
#define RTC_PIE BIT(6)
#define RTC_AIE BIT(5)
#define RTC_UIE BIT(4)

//Register C
#define RTC_C_REGISTER 0x0C
#define RTC_PI_PENDING BIT(6)
#define RTC_AI_PENDING BIT(5)
#define RTC_UI_PENDING BIT(4)

//Register D
#define RTC_D_REGISTER 0x0D

#endif /*_LCOM_I8042_H*/
