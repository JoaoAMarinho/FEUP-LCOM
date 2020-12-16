#include "rtc.h"

//uint8_t rtc_time[3];
//uint8_t rtc_date[3];

int rtc_hook_id=MOUSE_IRQ;

//---------------------------------------------------------------------------------------------

//functions
int (rtc_subscribe_int)(uint8_t *bit_n) {
  //hook_id_rtc = 8;
  *bit_n = rtc_hook_id;
    if (sys_irqsetpolicy(MOUSE_IRQ/*RTC*/ ,IRQ_REENABLE|IRQ_EXCLUSIVE, &rtc_hook_id)){
        printf("Irqpolicy fails");
        return 1;
    }

    //if (enable_update_interrupts(true) != 0) {return 1;}

    return 0;
}

int (rtc_unsubscribe_int)() {
   if (sys_irqrmpolicy(&rtc_hook_id)) {
        printf("Irqrmpolicy fails");
        return 1;
    }

    //if (enable_update_interrupts(false) != 0) {return 1;}

    return 0;
}
