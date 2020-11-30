// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "Videocard.h"
#include "keyboard.h"
#include "mouse.h"
#include "i8254.h"
#include "Player.h"
//#include "Game.h"
#include "Xpms/Main_Menu/BackGround.xpm"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

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
uint8_t keyboard_data;
bool kb_error=false;

int(proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])){
  int xi=0;
  int yi=0;

  vg_init(MODE3);

  xpm_image_t img;
	
	xpm_load(MainBackGround, XPM_8_8_8_8, &img);
  uint32_t* map=(uint32_t*)img.bytes;
  for(int i = 0; i < 800; i++) {
    for (int j = 0; j < 600; j++) {
      if (*(map + i + j*800) != xpm_transparency_color(XPM_8_8_8_8))
        drawPixel(xi+i,yi+j,*(map + i + j*800));
    }
  }
  free(&img);

  Player* player1=create_player(xi, yi, LEFT);
  draw_player(player1);

	int ipc_status, r;//counter=0;
	message msg;
	uint8_t kb_bit_no;//timer_bit_no;
  
	if (keyboard_subscribe_int(&kb_bit_no)) return 1;
	uint32_t kb_irq_set = BIT(kb_bit_no);

	//char *video_mem = get_video_mem();

	while ( keyboard_data!=ESC_KEY ) {
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
          animate_player(player1);
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

  free(player1);

	//Reset the video card to the text mode
	vg_exit();

    return 0;
}
/*
int(proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])) {
  
  if (vg_init(MODE3) != 0) {vg_exit(); return 1;}

  if (gameLoop() != 0) {return 1;}

  vg_exit();

  free_memory();

  return 0;
}
*/


