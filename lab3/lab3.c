#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <minix/sysutil.h>
#include "KeyBoard.h"


int cnt=0, itr_counter = 0;
uint8_t stat,data=0;



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
  keyboard_subscribe_int(&irq_set);
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
                  kbc_ih();
                  if(keyboard_read_command()) return 1;
                  //kbd_print_scancode(bool make, uint8_t size, uint8_t bytes[]);
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
  kbd_print_no_sysinb(cnt);
/*
  uint8_t irq_set, cmd=READ_COMMAND_BYTE;
  keyboard_subscribe_int(&irq_set);
  //stat, cmdc data
  int retry=3;
  while( retry!=0 ) {
    util_sys_inb(STATUS_REGISTER, &stat);
    if( (stat & INPUT_BUF_FULL) == 0 ) {
      sys_outb(COMMAND_REGISTER, cmd);
      return 0;
  }
  tickdelay(micros_to_ticks(DELAY_US));
  retry--;
}

retry=3;
while( retry!=0 ) {
  util_sys_inb(STATUS_REGISTER, &stat); 
  if( stat & OUTPUT_BUF_FULL ) {
    util_sys_inb(KEYBOARD_OUT_BUF, &data);
    if ( (stat &(PARITY_ERROR | TIMEOUT_ERROR)) == 0 )
      return data;
    else
      return -1;
  }
  tickdelay(micros_to_ticks(DELAY_US));
  retry--;
}
  keyboard_unsubscribe_int();*/
  printf ("%d",itr_counter);
  return 0;
}

int (kbd_test_poll) () {
  /* A ser preenchido pelos alunos */
  printf ("% s ainda não foi implementado! \n", __func__);

  return 1;
}

int (kbd_test_timed_scan) (uint8_t n) {
  /* A ser preenchido pelos alunos */
  printf ("% s ainda não foi implementado! \n", __func__);

  return 1;
}
