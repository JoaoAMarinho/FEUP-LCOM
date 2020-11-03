#include "mouse.h"

int hook_id=0;
extern bool error;
extern uint8_t received_data;

int(mouse_subscribe_int)(uint8_t* bit_n){
    *bit_n = BIT(hook_id);
    if (sys_irqsetpolicy(MOUSE_IRQ ,IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id)){
        printf("Irqpolicy fails");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)(){
    if (sys_irqrmpolicy(&hook_id)) {
        printf("Irqrmpolicy fails");
    return 1;
  }
  return 0;
}

void (mouse_ih)(){
    uint8_t stat;
    util_sys_inb(STATUS_REGISTER, &stat);
    if( stat & OUTPUT_BUF_FULL ) {
        if ( (stat &(PARITY_ERROR | TIMEOUT_ERROR | AUX)) != 0 ) error=true; //Need to verify AUX as well
        else
        util_sys_inb(OUT_BUF, &received_data);
        error=false;
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
    else pp->delta_x=pp->bytes[2];
}

int (mouse_disable_data_reporting)(){
    uint8_t status=0, ackn_type;

    while(1){
        if (util_sys_inb(STATUS_REGISTER, &status) != 0)return 1;
        if((status& (AUX|INPUT_BUF_FULL))==0){
            if(sys_outb(STATUS_REGISTER, WRITE_MOUSE_BYTE)!=0) return 1;
        }
        if(util_sys_inb(STATUS_REGISTER, &status) != 0)return 1;
        if((status& (AUX|INPUT_BUF_FULL))==0){
            if(sys_outb(WRITE_COMMAND_BYTE, MOUSE_DISABLE)!=0) return 1;
        }
        if (util_sys_inb(WRITE_COMMAND_BYTE, &ackn_type) != 0) {
				printf("Sys_inb failed\n");
				return 1;
			}
		if(ackn_type == ACK){ //if everything OK
			return 0;
		}
		else if(ackn_type == ERROR){ //second consecutive invalid byte
			return 1;
		}
    }
}
