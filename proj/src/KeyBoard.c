#include "KeyBoard.h"

extern uint8_t keyboard_data;
extern bool kb_error;
int kb_hook_id = 1;

//---------------------------------------------------------------------------------------------

// Subscribes keyboard
int(keyboard_subscribe_int)(uint8_t* bit_no){
  *bit_no = kb_hook_id;
  if (sys_irqsetpolicy(KBD_IRQ ,IRQ_REENABLE|IRQ_EXCLUSIVE, &kb_hook_id)){
    printf("Irqpolicy fails");
    return 1;
  }
  return 0;
}

// Unsubscribes keyboard
int (keyboard_unsubscribe_int)(){
  if (sys_irqrmpolicy(&kb_hook_id)) {
    printf("Irqrmpolicy fails");
    return 1;
  }
  return 0;
}

// Handles keyboard interrupts
void (kbc_ih)() {
  uint8_t stat;
  util_sys_inb(STATUS_REGISTER, &stat);
  kb_error=false;
  if(stat & OUTPUT_BUF_FULL) {
    util_sys_inb(KEYBOARD_OUT_BUF, &keyboard_data);
    if ( (stat &(PARITY_ERROR | TIMEOUT_ERROR)) != 0 ) kb_error=true;
    else kb_error=false;
  }
  else
    kb_error = true;
}


