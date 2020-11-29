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

void (getEsc)(){
  int ipc_status;
  uint16_t r;
  message msg;
  uint8_t bit_no;
  keyboard_subscribe_int(&bit_no);

  uint32_t irq_set = BIT(bit_no);
  
  while( data!=ESC_KEY) { 
    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih();
          }
          break;

        default:
            break; /* no other notifications expected: do nothing */
        }
    }
    else { /* received a standard message, not a notification */
    /* no standard messages expected: do nothing */
    }
  }

  keyboard_unsubscribe_int();
}
