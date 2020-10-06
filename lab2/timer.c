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
  uint8_t RB_command = TIMER_RB_CMD| TIMER_RB_COUNT_ | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);
  sys_outb(TIMER_CTRL,RB_command);
  util_sys_inb(TIMER_0+timer,st);

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val conf;
  switch(field){
    /*!< status */
    case tsf_all: conf.byte=st;

    /*!< initialization mode */
    case tsf_initial: conf.in_mode=st&24;

    /*!< counting mode: 0, 1,.., 5 */
    case tsf_mode:conf.count_mode=st&7;

    /*!< counting base, true if BCD */
    case tsf_base: conf.bcd=st&1;
  }
  timer_print_config(timer, field, conf);
  return 0;
}
