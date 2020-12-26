#include "mouse.h"
#include <math.h>

int mouse_hook_id=MOUSE_IRQ;
extern bool mouse_error;
extern uint8_t mouse_data;
extern Mouse_event m_event;
extern uint16_t horizontal_res, vertical_res;
extern xpm_image_t current_background; //background image so that the cursor is deleted
//extern xpm_image_t background_obstacles;
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

Mouse_event * get_mouse_event(struct packet * mouse_pack){
	static bool lb_down = false, mb_down = false, rb_down = false;

	if ((!lb_down||lb_down) && !rb_down && !mb_down && mouse_pack->lb && !mouse_pack->mb && !mouse_pack->rb) {
		lb_down = true;
		m_event=L_DOWN;
	}
	else if (!lb_down && !rb_down && !mb_down && mouse_pack->rb && !mouse_pack->mb && !mouse_pack->lb) {
		rb_down = true;
		m_event=R_DOWN;
	}
	else if (lb_down && !rb_down && !mb_down && !mouse_pack->lb && !mouse_pack->mb && !mouse_pack->rb) {
		lb_down = false;
		m_event=L_UP;
	}
	else if (!lb_down && rb_down && !mb_down && !mouse_pack->lb && !mouse_pack->mb && !mouse_pack->rb){
		rb_down = false;
		m_event=R_UP;
	}
	else if (!mb_down && mouse_pack->mb) {
		mb_down = true;
		m_event=MIDLE;
	}
	else if(mb_down && !mouse_pack->mb) {
		mb_down = false;
		m_event=MIDLE;
	}
	else 
    	m_event=MOVE;
        
  	return &m_event;
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
        if (cursor->x + mouse_pack->delta_x > horizontal_res - cursor->img.width)
            cursor->x = horizontal_res - cursor->img.width;
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
        if (cursor->y + cursor->img.height - mouse_pack->delta_y > vertical_res)
            cursor->y = vertical_res - cursor->img.height;
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

    for(int row = 0;row < cursor->img.height; row++){
        for(int column = 0;column < cursor->img.width; column++){
            if (*(map + column + row*cursor->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(cursor->x+column,cursor->y+row,*(map + column + row*cursor->img.width));
        }
    }
}

void erase_cursor(){
    uint32_t* map = (uint32_t*) current_background.bytes;
    uint32_t*cursorMap=(uint32_t*) cursor->img.bytes;
        
    for(int row =0;row < cursor->img.height; row++){
        for(int column = 0; column < cursor->img.width; column++){
            if (*(cursorMap + column + row*cursor->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(cursor->x+column,cursor->y+row,*(map + (cursor->x+column) + (cursor->y+row)*horizontal_res));
        }
    }
}

char checkOverMain(){
    //Verificar se o cursor está nalgum botão
    if (cursor->x > 545 && cursor->x < 735 && cursor->y > 180 && cursor->y < 230)
        return 'P';
    else if (cursor->x > 545 && cursor->x < 735 && cursor->y > 240 && cursor->y < 290)
        return 'I';  
    else if (cursor->x > 545 && cursor->x < 735 && cursor->y > 300 && cursor->y < 350)  
        return 'B';
    else if (cursor->x > 545 && cursor->x < 735 && cursor->y > 360 && cursor->y < 410)
        return 'E';
    else if(cursor->x >= 26 && cursor->x <= 73 && cursor->y >= 527 && cursor->y <= 576)
        return 'C';
    else 
        return 'N';
}

int checkOverIce(){
    int temp_x=cursor->x-315;
    int temp_y=cursor->y;
    //Over button 1
    if(cursor->x >=352 && cursor->x <= 431 && cursor->y>=261 && cursor->y<=401)
        return 1;
    else if(temp_x>=0 && temp_x<=37 && temp_y>=331+(temp_x*(-1.89)) && temp_y<=331)
        return 1;
    else if(temp_x>=0 && temp_x<=37 && temp_y>331 && temp_y<=331+(temp_x*(1.89)))
        return 1;
    temp_x=cursor->x-431;
    if(temp_x>=0 && temp_x<=37 && temp_y>=261+(temp_x*(1.89)) && temp_y<=331)
        return 1;
    else if(temp_x>=0 && temp_x<=37 && temp_y>331 && temp_y<=401+(temp_x*(-1.89)))
        return 1;
    //Over button 2
    temp_x=cursor->x-444;
    if(cursor->x >=481 && cursor->x <= 563 && cursor->y>=188 && cursor->y<=328)
        return 2;
    else if(temp_x>=0 && temp_x<=37 && temp_y>=258+(temp_x*(-1.89)) && temp_y<=258)
        return 2;
    else if(temp_x>=0 && temp_x<=37 && temp_y>258 && temp_y<=258+(temp_x*(1.89)))
        return 2;
    temp_x=cursor->x-563;
    if(temp_x>=0 && temp_x<=37 && temp_y>=188+(temp_x*(1.89)) && temp_y<=258)
        return 2;
    else if(temp_x>=0 && temp_x<=37 && temp_y>258 && temp_y<=328+(temp_x*(-1.89)))
        return 2;
    //Over button 3
    temp_x=cursor->x-446;
    if(cursor->x >=483 && cursor->x <= 562 && cursor->y>=338 && cursor->y<=478)
        return 3;
    else if(temp_x>=0 && temp_x<=37 && temp_y>=408+(temp_x*(-1.89)) && temp_y<=408)
        return 3;
    else if(temp_x>=0 && temp_x<=37 && temp_y>408 && temp_y<=408+(temp_x*(1.89)))
        return 3;
    temp_x=cursor->x-562;
    if(temp_x>=0 && temp_x<=37 && temp_y>=338+(temp_x*(1.89)) && temp_y<=408)
        return 3;
    else if(temp_x>=0 && temp_x<=37 && temp_y>408 && temp_y<=478+(temp_x*(-1.89)))
        return 3;
    //Not over any button
    return -1;
}

