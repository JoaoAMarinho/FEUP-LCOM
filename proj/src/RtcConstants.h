#ifndef _LCOM_RTCCONSTANTS_H_
#define _LCOM_RTCCONSTANTS_H_

#include <lcom/lcf.h>

#define RTC_IRQ 0x08

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

#endif
