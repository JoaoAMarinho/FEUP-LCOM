#include "keyboard.h"

extern int itr_counter, cnt;
extern uint8_t stat,data;
int hook_id = 0;



// Subscribes keyboard
int(keyboard_subscribe_int)(uint8_t* bit_no){
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ ,IRQ_REENABLE, &hook_id)){
    printf("Irqpolicy fails");
    return 1;
  }
  return 0;
}

// Unsubscribes keyboard
int (keyboard_unsubscribe_int)(){
  if (sys_irqrmpolicy(&hook_id)) {
    printf("Irqrmpolicy fails");
    return 1;
  }
  return 0;
}

// Handles keyboard interrupt counter
void (kbc_ih)() {
  itr_counter++;
}

int (keyboard_read_command)(){
  util_sys_inb(STATUS_REGISTER, &stat); 
  if( stat & OUTPUT_BUF_FULL ) {
    util_sys_inb(KEYBOARD_OUT_BUF, &data);
    if ( (stat &(PARITY_ERROR | TIMEOUT_ERROR)) == 0 )return 0;
    else
      return 1;
  }
  return 1;
}
