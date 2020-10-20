#include "keyboard.h"

extern uint8_t data;
int hook_id = 0;
extern bool error;



// Subscribes keyboard
int(keyboard_subscribe_int)(uint8_t* bit_no){
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(KBD_IRQ ,IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id)){
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
  uint8_t stat;
  util_sys_inb(STATUS_REGISTER, &stat);
  if( stat & OUTPUT_BUF_FULL ) {
    if ( (stat &(PARITY_ERROR | TIMEOUT_ERROR)) != 0 ) error=true;
    else
      util_sys_inb(KEYBOARD_OUT_BUF, &data);
      error=false;
  }
  error = true;
}
