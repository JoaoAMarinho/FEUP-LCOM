#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "videocard.h"
#include "keyboard.h"
#include "sprite.h"
#include "i8254.h"
#include "Pirate_bmp.xpm"
#include "proj.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int time_counter=0;
uint8_t data;
bool error=false;

int(proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])){
    int xi=0;
    int yi=0;
    xpm_map_t xpm=minix3_xpm;
    vg_init(MODE3);
	Sprite* sprite=create_sprite(xpm, xi, yi, 0, 0, XPM_8_8_8);
	draw_sprite(sprite,NULL);

	int ipc_status, r;//counter=0;
	message msg;
	uint8_t kb_bit_no;//timer_bit_no;
  
	if (keyboard_subscribe_int(&kb_bit_no)) return 1;
	uint32_t kb_irq_set = BIT(kb_bit_no);

	//char *video_mem = get_video_mem();

	while ( data!=ESC_KEY ) {
		if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          	printf("driver_receive failed with: %d", r);
          	continue;
      	}
		if (is_ipc_notify(ipc_status)) { 
          	switch (_ENDPOINT_P(msg.m_source)) {
          	case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & kb_irq_set) { 
					/* process it */
					kbc_ih();
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

	destroy_sprite(sprite);

	//Reset the video card to the text mode
	vg_exit();

    return 0;
}
