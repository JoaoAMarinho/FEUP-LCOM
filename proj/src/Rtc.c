#include "rtc.h"

int rtc_hook_id=RTC_IRQ;
extern uint8_t rtc_date[3];
extern uint16_t horizontal_res, vertical_res;
extern xpm_image_t current_background;

Date *date;

//---------------------------------------------------------------------------------------------

int (rtc_subscribe_int)(uint8_t *bit_n) {
  *bit_n = rtc_hook_id;
    if (sys_irqsetpolicy(RTC_IRQ ,IRQ_REENABLE|IRQ_EXCLUSIVE, &rtc_hook_id)){
        return 1;
    }

    if(rtc_enable_update_int(true)) return 1;

    return 0;
}

int (rtc_unsubscribe_int)() {
    if (sys_irqrmpolicy(&rtc_hook_id)){
        return 1;
    }

    if (rtc_enable_update_int(false)) return 1;

    return 0;
}

int rtc_enable_update_int(bool enable_disable){
    uint32_t registerB;

    if (sys_outb(RTC_ADDR_REG, RTC_B_REGISTER)) return 1;
    if (sys_inb(RTC_DATA_REG, &registerB)) return 1;
    if (enable_disable) registerB = RTC_UIE | registerB;
    else registerB = (~RTC_UIE) & registerB;

    if (sys_outb(RTC_ADDR_REG, RTC_B_REGISTER)) return 1;
    if (sys_outb(RTC_DATA_REG, registerB)) return 1;

    return 0;
}

int rtc_read_reg(uint8_t reg){
    if (not_valid_rtc()) return 1;

    if (reg != RTC_D && reg != RTC_M && reg != RTC_Y) {
        return 1;
    }

    uint32_t value;
    if (sys_outb(RTC_ADDR_REG, reg) != 0) return 1;
    if (sys_inb(RTC_DATA_REG, &value) != 0) return 1;
    
    uint8_t n=((uint8_t)value >> 4)*10;
    n+=((uint8_t)value & 0x0F);

    switch (reg) {
        case RTC_D:
            rtc_date[0] = n;
            break;
        case RTC_M:
            rtc_date[1] = n;
            break;
        case RTC_Y:
            rtc_date[2] = n;
            break;
        default:
            return 1;
            break;
    }

    return value;
}

bool not_valid_rtc() {
  uint32_t regA = 0;
  
  do {
    if (sys_irqdisable(&rtc_hook_id)) return true;
    if (sys_outb(RTC_ADDR_REG, RTC_A_REGISTER)) return true;
    if (sys_inb(RTC_DATA_REG, &regA)) return true;

    if (sys_irqenable(&rtc_hook_id)) return true;

  } while (regA & RTC_UIP);

  return false;
}

void LoadRTC() {
    rtc_read_reg(RTC_D);
    rtc_read_reg(RTC_M);
    rtc_read_reg(RTC_Y);
}

//Date
Date* create_date(){
    date = (Date*) malloc(sizeof(Date));

    date->day = 0;
    date->month = 0;
    date->year = 0;

    xpm_image_t img;

    xpm_load(Slash_xpm, XPM_8_8_8_8, &img);
    date->Symbols[0] = img;

    free(&img);

    return date;
}

void updateDate(){
    date->day = rtc_date[0];
    date->month = rtc_date[1];
    date->year = rtc_date[2];
}
