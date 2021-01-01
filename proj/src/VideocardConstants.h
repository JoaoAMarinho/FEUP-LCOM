#ifndef _LCOM_VIDEOCARDCONSTANTS_H_
#define _LCOM_VIDEOCARDCONSTANTS_H_

#include <lcom/lcf.h>

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


#endif
