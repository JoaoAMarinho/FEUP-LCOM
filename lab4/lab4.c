// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab4.h>

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
    int ipc_status, r, packet_counter=0;
    message msg;
    uint8_t irq_set;
    struct packet* pp=NULL;
    uint32_t counter=0;

    mouse_enable_data_reporting(); //criar nossa

    if(mouse_subscribe_int(&irq_set)) return 1;
    while(counter < cnt) { 
    /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
        }
        printf("subs 2\n");
        if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
            if (msg.m_notify.interrupts & irq_set) {//code here
             printf("kikikikik");
              mouse_ih();
              if(!error){
                if(packet_counter==0 && (received_data & A2_LINE)!=0){
                    pp->bytes[0]=received_data;
                    packet_counter++;
                }
                else if(packet_counter==1){
                    pp->bytes[1]=received_data;
                    packet_counter++;
                }
                else if(packet_counter==2){
                    pp->bytes[2]=received_data;
                    packet_counter=0;
                    get_packet(pp);
                    mouse_print_packet(pp);
                    counter++;
                }
            }
            }
            break;
            default:
              break; /* no other notifications expected: do nothing */
        }
      } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
      }
  }
  mouse_unsubscribe_int();
  printf("jhjhjh");
  mouse_disable_data_reporting(); //criar nossa
  printf("popopo");

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
