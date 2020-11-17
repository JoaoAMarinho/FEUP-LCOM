#include "mouse.h"
#include <math.h>

int mouse_hook_id=MOUSE_IRQ;
extern bool error;
extern uint8_t received_data;

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
    error = false;
    if( stat & OUTPUT_BUF_FULL ) {
        if ((stat &(PARITY_ERROR | TIMEOUT_ERROR)) != 0 ) {error=true;}
        else { error=false; }
        util_sys_inb(OUT_BUF, &received_data);
    }
    else error = true;
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

int (mouse_polling)() {
    struct packet pp;
    uint8_t stat;

    util_sys_inb(STATUS_REGISTER, &stat);
    if ((stat & OUTPUT_BUF_FULL) != 1) return 1;
    else if ((stat &(PARITY_ERROR | TIMEOUT_ERROR)) != 0 ) {return 1;}

    
    
    for (int i = 0; i < 3; i++ ) {
        util_sys_inb(OUT_BUF, &received_data);
        if(i==0){
            if((received_data & A2_LINE)!=0){
                pp.bytes[0]=received_data;
            }
            else continue;
        }
        else if(i==1){
            pp.bytes[1]=received_data;

        }
        else{
            pp.bytes[2]=received_data;
            get_packet(&pp);
            mouse_print_packet(&pp);
        }
    }
    return 0;
}

enum event (mouse_get_event)(struct packet *pp) {
    enum event result=0;
    if (pp->lb && !pp->mb && !pp->rb) {
        result = L_DOWN;
    }
    else if (!pp->lb && !pp->mb && !pp->rb) {
        result = B_UP;
    }
    else if (!pp->lb && !pp->mb && pp->rb) {
        result = R_DOWN;
    }
    else if (!pp->lb && pp->mb && !pp->rb) {
        result = M_DOWN;
    }
    return result;
}

void (gesture_handler)(struct packet *pp, uint8_t x_len, uint8_t tolerance, enum event m_event, bool *done) {
    static uint16_t x_delta = 0, y_delta = 0;
    static enum state current_state = INITIAL;


    switch(current_state) {
        case INITIAL: {
            x_delta = 0;
            y_delta = 0;
            if (m_event == L_DOWN) {
                current_state = MOVE_LEFT;
            }
            break;
        }
        case MOVE_LEFT: {
            if (m_event == B_UP) {
                if ((x_delta >= x_len) && (fabs(y_delta/(float)x_delta) > 1) ) {
                    current_state = SWITCH_SIDE;
                }
                else {
                    current_state = INITIAL;
                }
            }
            else if (m_event == L_DOWN) {
                if ((pp->delta_x > 0 && pp->delta_y > 0) || (abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance)) {
                    x_delta += pp->delta_x;
                    y_delta += pp->delta_y;
                }
                else current_state = INITIAL;
            }
            break;
        }
        case SWITCH_SIDE: {
            x_delta = 0;
            y_delta = 0;
            if (m_event == L_DOWN || m_event == M_DOWN ) {
                current_state = INITIAL;
            }
            else if(pp->delta_x!=0 && pp->delta_y !=0){
                current_state = INITIAL;
                printf("alguma coisa\n");
            }
            /*else if (!((pp->delta_x > 0 && pp->delta_y > 0) || (abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance))) {
                current_state = INITIAL;
            }*/
            else if (m_event == R_DOWN) {
                current_state = MOVE_RIGHT;
                printf("vou me mutar\n");
            }
            break;
        }
        case MOVE_RIGHT: {
            if (m_event == B_UP) {
                if ((x_delta >= x_len) && (fabs(y_delta/(float)x_delta) > 1) ) {
                    *done = true;
                }
                else {
                    current_state = INITIAL;
                }
            }
            else if (m_event == R_DOWN) {
                if ((pp->delta_x > 0 && pp->delta_y > 0) || (abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance)) {
                    x_delta += pp->delta_x;
                    y_delta += pp->delta_y;
                }
                else current_state = INITIAL;
            }
            break;
        }
    }

}

