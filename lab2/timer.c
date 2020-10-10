#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t RB_command = TIMER_RB_CMD| TIMER_RB_COUNT_  | TIMER_RB_SEL(timer);
  sys_outb(TIMER_CTRL,RB_command);
  if(util_sys_inb(TIMER_0+timer,st)!=0) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val conf;
  switch(field){
    /*!< status */
    case tsf_all: conf.byte=st;

    /*!< initialization mode */
    case tsf_initial:switch(st&48){
      case 0:conf.in_mode=INVAL_val;
      case 16:conf.in_mode=LSB_only;
      case 32:conf.in_mode=MSB_only;
      case 48:conf.in_mode=MSB_after_LSB;
    }
    
    /*!< counting mode: 0, 1,.., 5 */
    case tsf_mode:switch(st&14){
      case 0:conf.count_mode=0;
      case 2:conf.count_mode=1;
      case 4:
      case 12:
      conf.count_mode=2;
      case 6:
      case 14:
      conf.count_mode=3;
      case 8:conf.count_mode=4;
      case 10:conf.count_mode=5;
    }

    /*!< counting base, true if BCD */
    case tsf_base: conf.bcd=st&1;
  }
  if(!timer_print_config(timer, field, conf))return 1;
  return 0;
}
