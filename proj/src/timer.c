#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

extern int time_counter;
int timer_hook_id = 0;

//---------------------------------------------------------------------------------------------

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st;
  if(timer_get_conf(timer,&st)) return 1;

  uint8_t new_st = (st&0x0F) | TIMER_LSB_MSB;
  
  if (freq>TIMER_FREQ)return 1;
  uint16_t div = TIMER_FREQ/freq;

  uint8_t lsb, msb;
	
	util_get_LSB(div, &lsb);
	util_get_MSB(div, &msb);

  switch (timer)
	{
	case 0:
		new_st = new_st | TIMER_SEL0;
		if(sys_outb(TIMER_CTRL, new_st))return 1;
			
		if(sys_outb(TIMER_0, lsb))return 1;
			
		if(sys_outb(TIMER_0, msb))return 1;
		
		break;
	
	case 1:
		new_st = new_st | TIMER_SEL1;
		if (sys_outb(TIMER_CTRL, new_st))return 1;
			
		if(sys_outb(TIMER_1, lsb))return 1;
			
		if(sys_outb(TIMER_1, msb))return 1;
	
		break;
	
	case 2:
		new_st = new_st | TIMER_SEL2;	
		if(sys_outb(TIMER_CTRL, new_st))return 1;
			
		if(sys_outb(TIMER_2, lsb))return 1;
			
		if(sys_outb(TIMER_2, msb))return 1;
	
		break;

	default:
		return 1;
	}

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timer_hook_id;
  if (sys_irqsetpolicy(TIMER0_IRQ ,IRQ_REENABLE, &timer_hook_id) !=0 ){
    printf("Irqpolicy fails");
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) {
    printf("Irqrmpolicy fails");
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  time_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t RB_command = TIMER_RB_CMD| TIMER_RB_COUNT_  | TIMER_RB_SEL(timer);
  sys_outb(TIMER_CTRL,RB_command);
  if(util_sys_inb(TIMER_0+timer,st)) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  union timer_status_field_val conf;
  switch(field){
    /*!< status */
    case tsf_all:
      conf.byte=st;
      break;

    /*!< initialization mode */
    case tsf_initial:
      conf.in_mode=(st&TIMER_LSB_MSB)>>4;
      break;
    
    /*!< counting mode: 0, 1,.., 5 */
    case tsf_mode:
      st=(st&(TIMER_SQR_WAVE|BIT(3)))>>1;
      if(st==6||st==7) st-=4;
      conf.count_mode=st;
      break;

    /*!< counting base, true if BCD */
    case tsf_base: 
      conf.bcd=st&1;
      break;
  }
  if(timer_print_config(timer, field, conf))return 1;

  return 0;
}
