// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "videocard.h"
#include "keyboard.h"
#include "sprite.h"
#include "i8254.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

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

int(video_test_init)(uint16_t mode, uint8_t delay) {
    if(set_mode(mode)!=0) return 1;
    sleep(delay);
    if(vg_exit()!=0) return 1;
    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color) {

  //Map video RAM && Change to graphics "mode"
  vg_init(mode);

  vg_draw_rectangle(x,y,width,height,color);

  //Subscribe keyboard interrups
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

	//Reset the video card to the text mode
	vg_exit();

	return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

	//Map video RAM && Change to graphics "mode"
	vg_init(mode);

	unsigned rectangleWidth=getHorizontal()/no_rectangles;  //All the rectangles width 
	unsigned rectangleHeight=getVertical()/no_rectangles;   //All the rectangles height

	uint32_t color=first; // First color to be used

	for(int i=0; i< no_rectangles ; i++){    //Rows
		for(int j=0 ; j<no_rectangles ; j++){  //Columns

		get_color(&color, i, j, no_rectangles, first, step);

		vg_draw_rectangle(j*rectangleWidth,i*rectangleHeight,rectangleWidth,rectangleHeight,color);
		}
	}

	getEsc();

	//Reset the video card to the text mode
	vg_exit();

	return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

	vg_init(MODE1);
	/*
	vg_init(MODE5);
	xpm_image_t img1;

    xpm_load(img, XPM_8_8_8_8, &img1);
	uint32_t* map=(uint32_t*)img1.bytes;

	for(int i = 0; i < img1.width; i++) {
            for (int j = 0; j < img1.height; j++) {
            if (*(map + i + j*img1.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(i,j,*(map + i + j*img1.width));
            }
        }
	*/
	/* Function without sprites

	xpm_image_t img;
	
	xpm_load(xpm, XPM_INDEXED, &img);

	drawXpm(x,y,&img);
	*/

	Sprite* sprite=create_sprite(xpm, x, y, 0, 0, XPM_INDEXED);

	draw_sprite(sprite,NULL);

	getEsc();

	destroy_sprite(sprite);

	//Reset the video card to the text mode
	vg_exit();

	return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,int16_t speed, uint8_t fr_rate) {

	vg_init(MODE1);
	Sprite* sprite=create_sprite(xpm, xi, yi, 0, 0, XPM_INDEXED);
	draw_sprite(sprite,NULL);

	int ipc_status, r,counter=0;
	message msg;
	uint8_t kb_bit_no, timer_bit_no;
  
	if (keyboard_subscribe_int(&kb_bit_no)) return 1;
	uint32_t kb_irq_set = BIT(kb_bit_no);

	if (timer_subscribe_int(&timer_bit_no)) return 1;

	uint32_t timer_irq_set = BIT(timer_bit_no);

	int interrupts_to_frame = 60 / fr_rate;

	char *video_mem = get_video_mem();

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
					/* Code to move right if you click 'd'
					if(data==0x20){
						memset(video_mem, 0, get_bytes_per_pixel() * getHorizontal() * getVertical());  //Clear the screen
						xi += 3; //Go right
						sprite->x = xi;
						draw_sprite(sprite, NULL);
					}*/
					
				}
				if (msg.m_notify.interrupts & timer_irq_set) {
					counter++;
					if(counter%interrupts_to_frame==0){
						if (((xi % getHorizontal()) == xf && (yi % getVertical()) == yf) || xi > getHorizontal() || yi > getVertical())
              				continue;
						if(speed<0){
							timer_int_handler();
							if (time_counter == abs(speed)){
								memset(video_mem, 0, get_bytes_per_pixel() * getHorizontal() * getVertical());  //Clear the screen
								if (xi == xf) {  //Move in the y axis
									if(yf>yi){
										yi += 1; //Go down
									}else{
										yi -= 1; //Go up
									}
								}
								else if(yi == yf) { //Move in the x axis
									if(xf>xi){
										xi += 1; //Go right
									}else{
										xi -= 1; //Go left
									}
								}/*
								else{ //Move in both axis
									if(yf>yi){
										yi += 1; //Go down
									}else{
										yi -= 1; //Go up
									}
									if(xf>xi){
										xi += 1; //Go right
									}else{
										xi -= 1; //Go left
									}
								}*/
								sprite->x = xi;
								sprite->y = yi;
								draw_sprite(sprite, NULL);

								time_counter=0;
							}
						}
						else{
							memset(video_mem, 0, get_bytes_per_pixel() * getHorizontal() * getVertical());  //Clear the screen
							if (xi == xf) {  //Move in the y axis
								if(abs(yf-yi)<speed) speed=abs(yf-yi); //Case it is the last displacement and the speed is higher than the movement
								if(yf>yi){
									yi += speed; //Go down
								}else{
									yi -= speed; //Go up
								}
							}
							else if(yi == yf) { //Move in the x axis
								if(abs(xf-xi)<speed) speed=abs(xf-xi); //Case it is the last displacement and the speed is higher than the movement
								if(xf>xi){
									xi += speed; //Go right
								}else{
									xi -= speed; //Go left
								}
							}/*
							else{ //Move in both axis
								if(yf>yi){
									yi += speed; //Go down
								}else{
									yi -= speed; //Go up
								}
								if(xf>xi){
									xi += speed; //Go right
								}else{
									xi -= speed; //Go left
								}
							}*/
							sprite->x = xi;
							sprite->y = yi;
							draw_sprite(sprite, NULL);

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
	keyboard_unsubscribe_int();
	timer_unsubscribe_int();

	destroy_sprite(sprite);

	//Reset the video card to the text mode
	vg_exit();

  	return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
