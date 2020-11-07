// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"

uint8_t received_data;
bool error=false;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  message msg;
  int ipc_status, packet_counter=0;
  int r; 
	uint8_t bit_no;
  struct packet pp;
  uint32_t counter=0;

  mouse_data_reporting(MOUSE_ENABLE);

  mouse_subscribe_int(&bit_no);
  uint32_t irq_set=BIT(bit_no);

  while(counter < cnt) {
    
    if ((r = driver_receive(ANY, &msg, &ipc_status))) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
            if(error) return 1;
            if(packet_counter==0){
              if((received_data & A2_LINE)!=0){
                pp.bytes[0]=received_data;
                packet_counter++;
              }
              else continue;
            }
            else if(packet_counter==1){
              pp.bytes[1]=received_data;
              packet_counter++;
            }
            else{
              pp.bytes[2]=received_data;
              packet_counter=0;
              get_packet(&pp);
              mouse_print_packet(&pp);
              counter++;
            }
          }
          break;
          default:
            break; /* no other notifications expected: do nothing */
        }
    }
  }
  mouse_unsubscribe_int();
  mouse_data_reporting(MOUSE_DISABLE);

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  /* To be completed */
  printf("%s(%u): under construction\n", __func__, idle_time);
  return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  /* To be completed */
  printf("%s: under construction\n", __func__);
  return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
