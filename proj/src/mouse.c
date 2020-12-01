#include "mouse.h"
#include <math.h>

int mouse_hook_id=MOUSE_IRQ;
extern bool mouse_error;
extern uint8_t mouse_data;
extern uint16_t horizontal_res, vertical_res;
//extern xpm_image_t background_menu; background image so that the cursor is deleted
//extern Room * room;

Cursor *cursor;

//---------------------------------------------------------------------------------------------

int(mouse_subscribe_int)(uint8_t* bit_n){
    *bit_n = mouse_hook_id;
    if (sys_irqsetpolicy(MOUSE_IRQ ,IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id)){
        printf("Irqpolicy fails");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)(){
    if (sys_irqrmpolicy(&mouse_hook_id)) {
        printf("Irqrmpolicy fails");
    return 1;
  }
  return 0;
}

void (mouse_ih)(){
    uint8_t stat;
    util_sys_inb(STATUS_REGISTER, &stat);
    mouse_error = false;
    if( stat & OUTPUT_BUF_FULL ) {
        if ((stat &(PARITY_ERROR | TIMEOUT_ERROR)) != 0 ) {mouse_error=true;}
        else { mouse_error=false; }
        util_sys_inb(MOUSE_OUT_BUF, &mouse_data);
    }
    else mouse_error = true;
}

void (get_packet)(struct packet *pp){
    int y_msb, x_msb;
    pp->lb=pp->bytes[0]&LEFT_BUTTON;
    pp->rb=(pp->bytes[0] & RIGHT_BUTTON) >> 1;
    pp->mb=(pp->bytes[0]&MIDDLE_BUTTON) >> 2;
    pp->x_ov=(pp->bytes[0] & X_OVFL) >> 6;
    pp->y_ov=(pp->bytes[0] & Y_OVFL) >> 7;
    x_msb=(pp->bytes[0] & X_SIGN) >> 4;
    y_msb=(pp->bytes[0] & Y_SIGN) >> 5;
    if(x_msb) pp->delta_x=(pp->bytes[1]-256);
    else pp->delta_x=pp->bytes[1];
    if(y_msb) pp->delta_y=(pp->bytes[2]-256);
    else pp->delta_y=pp->bytes[2];
}

int (mouse_data_reporting)(uint32_t cmd){
    uint8_t status=0, ackn_type;

    while(1){
        if (util_sys_inb(STATUS_REGISTER, &status) != 0)return 1;

        if((status & (AUX|INPUT_BUF_FULL))==0){
            if(sys_outb(STATUS_REGISTER, WRITE_MOUSE_BYTE)!=0) return 1;
        }

        if(util_sys_inb(STATUS_REGISTER, &status) != 0)return 1;

        if((status & (AUX|INPUT_BUF_FULL))==0){
            if(sys_outb(WRITE_COMMAND_BYTE, cmd)!=0) return 1;
        }

        if (util_sys_inb(WRITE_COMMAND_BYTE, &ackn_type) != 0) return 1;
			
		if(ackn_type == ACK){ //if everything OK
			return 0;
		}
		else if(ackn_type == ERROR){ //second consecutive invalid byte
			return 1;
		}
    }
    return 0;
}

//Cursor
Cursor * create_cursor(){
    cursor = (Cursor *) malloc(sizeof(Cursor));

    xpm_load(cursor_img, XPM_8_8_8_8, &cursor->img);

    cursor->x = 400;
    cursor->y = 300;

  return cursor;
}

void update_cursor(struct packet * mouse_pack){
    //Apagar o cursor
    erase_cursor();

    if (mouse_pack->delta_x > 0) {
        if (cursor->x + mouse_pack->delta_x > (int)horizontal_res - cursor->img.width)
            cursor->x = (int)horizontal_res - cursor->img.width;
        else
            cursor->x += mouse_pack->delta_x;
    }
    else if (mouse_pack->delta_x < 0) {
        if (cursor->x + mouse_pack->delta_x < 0)
            cursor->x = 0;
        else
            cursor->x += mouse_pack->delta_x;
    }
    if (mouse_pack->delta_y < 0) {
        if (cursor->y + cursor->img.height - mouse_pack->delta_y > (int)vertical_res)
            cursor->y = (int)vertical_res - cursor->img.height;
        else
            cursor->y -= mouse_pack->delta_y;
    }
    else if (mouse_pack->delta_y > 0) {
        if (cursor->y - mouse_pack->delta_y < 0)
            cursor->y = 0;
        else
            cursor->y -= mouse_pack->delta_y;
    }

    draw_cursor();
}

void draw_cursor(){
    uint32_t* map = (uint32_t*) cursor->img.bytes;

    for(int i = 0; i < cursor->img.width; i++) {
        for (int j = 0; j < cursor->img.height; j++) {
          if (*(map + i + j*cursor->img.width) != xpm_transparency_color(XPM_8_8_8_8))
            drawPixel(cursor->x+i,cursor->y+j,*(map + i + j*cursor->img.width));
    }
  }
}

void erase_cursor(){
    //uint32_t* map = (uint32_t*) background_menu.bytes;

    for (int i = cursor->x; i <= cursor->x + cursor->img.width; i++) {
        for (int j = cursor->y; j <= cursor->y + cursor->img.height; j++) {
            if (i < (int)horizontal_res - 1 && j < (int)vertical_res - 1)
                //drawPixel(i,j,*(room->back + i + j * horizontal_res));
                break; //Só para make purposes
    }
  }
}

