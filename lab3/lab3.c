#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <minix/sysutil.h>
#include "KeyBoard.h"




int cnt=0;
uint8_t data;
bool error=false;
int counter;



int main (int argc, char * argv []) {
  // define o idioma das mensagens LCF (pode ser EN-US ou PT-PT)
  lcf_set_language ("EN-US");

  // permite registrar invocações de função que estão sendo "empacotadas" pelo LCF
  // [comente se você não quiser / precisar]
  lcf_trace_calls ("/home/lcom/labs/g02/lab3/trace.txt");

  // permite salvar a saída de chamadas de função printf em um arquivo
  // [comente se você não quiser / precisar]
  lcf_log_output ("/home/lcom/labs/g02/lab3/output.txt");

  // lida com o controle do LCF
  // [LCF lida com argumentos de linha de comando e invoca a função certa]
  if (lcf_start (argc, argv))
    return 1;

  // LCF limpar tarefas
  // [deve ser a última instrução antes do retorno]
  lcf_cleanup ();

  return 0;
}

int (kbd_test_scan) () { 
  int ipc_status, r;
  message msg;
  uint8_t irq_set;
  uint8_t size=0;
  uint8_t vect[2];
  bool make;
  if(keyboard_subscribe_int(&irq_set)) return 1;
  while( data!=ESC_KEY) { 
  /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
					/* process it */
					size=1;
					kbc_ih();
					if(!error){
						vect[0]=data;
						if(data==TWO_BYTES){
						kbc_ih();
						size++;
						vect[1]=data;
					}
					if(data & MAKE_CODE_VERIFY) 
					{
						make = false;
					}
					else make = true;
					if(kbd_print_scancode(make, size, vect)) return 1;
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
  keyboard_unsubscribe_int();
  if (kbd_print_no_sysinb(cnt)) return 1;
  return 0;
}

int (kbd_test_poll) () {
  uint8_t size=0;
  uint8_t vect[2];
  bool make;
  while( data!=ESC_KEY ) { 
    kbc_poll_ih();
    size=1;
    if(!error){
      vect[0]=data;
      if(data==TWO_BYTES){
        kbc_poll_ih();
        size++;
        vect[1]=data;
      }
      if(data & MAKE_CODE_VERIFY) 
      {
        make = false;
      }
      else make = true;
      if(kbd_print_scancode(make, size, vect)) return 1;
    }
  }
  if(itrp_enable()) return 1;
  if (kbd_print_no_sysinb(cnt)) return 1;
  return 0;
}

int (kbd_test_timed_scan) (uint8_t n) {
	int ipc_status, r;
	message msg;
	uint8_t kb_irq_set, timer_irq_set;
	uint8_t size=0;
  	uint8_t vect[2];
  	bool make;
  
	if (keyboard_subscribe_int(&kb_irq_set)) return 1;

	if (timer_subscribe_int(&timer_irq_set)) return 1;

	while ( data!=ESC_KEY && counter < n*60 ) {
		if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          	printf("driver_receive failed with: %d", r);
          	continue;
      	}
		if (is_ipc_notify(ipc_status)) { 
          	switch (_ENDPOINT_P(msg.m_source)) {
          	case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & kb_irq_set) { 
					/* process it */
					size=1;
					kbc_ih();
					if(!error){
						vect[0]=data;
						if(data==TWO_BYTES){
						kbc_ih();
						size++;
						vect[1]=data;
					}
					if(data & MAKE_CODE_VERIFY) 
					{
						make = false;
					}
					else make = true;
					if(kbd_print_scancode(make, size, vect)) return 1;
					}
					counter = 0;
				}
				if (msg.m_notify.interrupts & timer_irq_set) {
					timer_int_handler();
					if (counter % 60 == 0) {
						timer_print_elapsed_time();
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
	keyboard_unsubscribe_int();
	if (kbd_print_no_sysinb(cnt)) return 1;
	timer_unsubscribe_int();
	return 0;
}
