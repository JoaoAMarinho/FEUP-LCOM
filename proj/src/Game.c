#include "Game.h"

//Device global variables

//Timer
unsigned int time_counter=0;
//KeyBoard
uint8_t keyboard_data;
bool kb_error=false;
//Mouse
struct packet mouse_pack;
int mouse_packet_index=0;
struct mouse_ev mouse_event;
bool mouse_error=false;
//Graphics card
extern uint16_t horizontal_res, vertical_res;

//Game global variables
static Player * player;
static Room * room;
extern Cursor * cursor;
extern DateTime * dateTime;
Menu gameMenu = MAIN;

//static Lever * leverBeingUsed;
//static xpm_image_t LeverPositions[7];
//static xpm_image_t LeverColors[2];
//static xpm_image_t CleanLever;

//---------------------------------------------------------------------------------------------

int gameLoop(){
    int ipc_status, r;
	message msg;

    uint8_t timer_bit_no, kb_bit_no, mouse_bit_no, rtc_bit_no;

    //Subscribes
    //if (timer_subscribe_int(&timer_bit_no) != 0) {return 1;}
    if (keyboard_subscribe_int(&kb_bit_no) != 0) {return 1;}
    //if (rtc_subscribe_int(&rtc_bit_no) != 0) {return 1;}
    if(mouse_data_reporting(MOUSE_ENABLE) !=0 ){return 1;}
    if (mouse_subscribe_int(&mouse_bit_no); != 0) {return 1;}

    //uint32_t timer_irq_set = BIT(timer_bit_no);
    //uint32_t kb_irq_set = BIT(kb_bit_no);
    //uint32_t mouse_irq_set = BIT(mouse_bit_no);
    //uint32_t rtc_irq_set = BIT(rtc_bit_no);

    uint8_t kb_bytes[2];

    while(gameMenu != FINAL) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    //Keyboard interrupt
                    if (msg.m_notify.interrupts & kb_irq_set) {
					    kbc_ih();
					    if(!kb_error){
						    kb_bytes[0]=keyboard_data;
						    if(data==TWO_BYTES){
						        kbc_ih();
						        kb_bytes[1]=keyboard_data;
					        }
					        if(data & MAKE_CODE_VERIFY) {/*make = false;*/}
					        else make = true;
					    }
                        receiveInterrupt(KEYBOARD);
                    }
                    //Mouse interrupt
                    if (msg.m_notify.interrupts & mouse_irq_set) {
                        mouse_ih();
				        if(error) return 1;
				        if(mouse_packet_index==0){
				            if((received_data & A2_LINE)!=0){
					            mouse_pack.bytes[0]=received_data;
					            mouse_packet_index++;
				            }
				            else continue;
				        }
				        else if(mouse_packet_index==1){
				            mouse_pack.bytes[1]=received_data;
				            mouse_packet_index++;
				        }
				        else{
				            mouse_pack.bytes[2]=received_data;
				            mouse_packet_index=0;
				            //mouse_event = mouse_get_event(&pp);
				            //gesture_handler(&pp,x_len,tolerance,mouse_event,&done);
				        }
                        receiveInterrupt(MOUSE);
                    }
                    //RTC interrupt
                    if (msg.m_notify.interrupts & rtc_irq_set) {
                        rtc_ih();
                        receiveInterrupt(RTC);
                    }
                    //Timer interrupt
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_int_handler();
                        receiveInterrupt(TIMER);
                        //double_buffer();
                    } 
                    break;
                default:
                break; 
            }
        }
    }

  //addToTransmitQueue(VM_DISCONNECTED);

  //Unsubscribes
  if (mouse_unsubscribe_int() != 0) {return 1;}
  if (mouse_disable_data_report() != 0) {return 1;}
  if (rtc_unsubscribe_int() != 0) {return 1;}
  if (kbd_unsubscribe_int() != 0) {return 1;}
  if (timer_unsubscribe_int() != 0) {return 1;}

  //free(level);

  return 0;

    
}

void receiveInterrupt(Device device);

void interruptHandler(Device device);
