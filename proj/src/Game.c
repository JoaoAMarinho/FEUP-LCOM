#include "Game.h"

//Device global variables
//Timer
unsigned int time_counter=0;
unsigned int game_counter=300;
//KeyBoard
uint8_t keyboard_data;
bool kb_error=false;
//Mouse
uint8_t mouse_data;
struct packet mouse_pack;
int mouse_packet_index=0;
Mouse_event m_event;
bool mouse_error=false;
//Graphics card
uint16_t horizontal_res, vertical_res;

//Game global variables
extern Player * player;
extern Room * room;
extern Cursor * cursor;
extern Task ** gameTasks;
extern int n_tasks;
extern Opponent ** gameOpponents;
extern int n_opponents;
extern GameTimer* gameTimer;
//extern Date * date;
Menu gameMenu = MAIN;


//---------------------------------------------------------------------------------------------

int gameLoop(){
  
    int ipc_status, r;
  	message msg;

    uint8_t timer_bit_no, kb_bit_no, mouse_bit_no, rtc_bit_no=0;

    //Subscribes
    if (timer_subscribe_int(&timer_bit_no) != 0) {return 1;}
    if (keyboard_subscribe_int(&kb_bit_no) != 0) {return 1;}
    //if (rtc_subscribe_int(&rtc_bit_no) != 0) {return 1;}
    if(mouse_data_reporting(MOUSE_ENABLE) !=0 ){return 1;}
    if (mouse_subscribe_int(&mouse_bit_no) != 0) {return 1;}

    uint32_t timer_irq_set = BIT(timer_bit_no);
    uint32_t kb_irq_set = BIT(kb_bit_no);
    uint32_t mouse_irq_set = BIT(mouse_bit_no);
    uint32_t rtc_irq_set = BIT(rtc_bit_no);

    LoadMain();
	LoadGameTimer();
    //Other loads (tasks and other things)
	LoadOpponents(); //Load to gameOpponents array
	LoadTasks(); //Load to gameTasks array
    //date = load_date();
    //LoadRtc();

    uint8_t kb_bytes[2];

    while(gameMenu != FINAL && keyboard_data != ESC_KEY) {
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
						            if(keyboard_data==TWO_BYTES){
						              kbc_ih();
						              kb_bytes[1]=keyboard_data;
					              }
					              /*if(data & MAKE_CODE_VERIFY) {make = false;} break code
					              else make = true;                           make code*/
                        receiveInterrupt(KEYBOARD);
					            }
                    }
                    //Mouse interrupt
                    if (msg.m_notify.interrupts & mouse_irq_set) {
                      mouse_ih();
				              if(mouse_error) return 1;
				              if(mouse_packet_index==0){
				                if((mouse_data & A2_LINE)!=0){
					                mouse_pack.bytes[0]=mouse_data;
					                mouse_packet_index++;
				                }
				                else continue;
				              }
				              else if(mouse_packet_index==1){
				                  mouse_pack.bytes[1]=mouse_data;
				                  mouse_packet_index++;
				              }
				              else{
				                mouse_pack.bytes[2]=mouse_data;
				                mouse_packet_index=0;
                        		get_packet(&mouse_pack);
				                receiveInterrupt(MOUSE);
				              }
                    }
                    //RTC interrupt
                    if (msg.m_notify.interrupts & rtc_irq_set) {
                        //rtc_ih();
                        //receiveInterrupt(RTC);
                    }
                    //Timer interrupt
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_int_handler();
                        receiveInterrupt(TIMER);
                        copy_to_vram();
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
  if (mouse_data_reporting(MOUSE_DISABLE) != 0) {return 1;}
  //if (rtc_unsubscribe_int() != 0) {return 1;}
  if (keyboard_unsubscribe_int() != 0) {return 1;}
  if (timer_unsubscribe_int() != 0) {return 1;}

  delete_room(room);

  return 0;

    
}

void receiveInterrupt(Device device){
    switch (gameMenu){
        case MAIN:
            Main_ih(device);
            break;
        case PLAYING:
            Play_ih(device);
            break;
        case BESTSCORES:
            Bestscores_ih(device);
            break;
        case INSTRUCTIONS:
            Instructions_ih(device);
            break;
        /*Tasks cases*/
		case ICETASK:
			Ice_ih(device);
			break;
		case SHIPTASK:
			Ship_ih(device);
			break;
        //case LEVER:
        //    LeverInterruptHandler(device);
        //    break;
        case PAUSE:
            Pause_ih(device);
            break;
        case GAMEMAP:
            GameMap_ih(device);
            break;
        case VICTORY:
            Victory_ih(device);
            break;
        case DEFEAT:
            Defeat_ih(device);
            break;
        case FINAL:
            break;
  }
}

void Play_ih(Device device){
    static Projectile * playerProjectile;
	static bool pr_exists=false , canBlast=true, projectile_anim=false, opponent_anim=false, victory;
	static int projectile_index=0, opponent_anim_index=0, opponent_index;
    //static int checkLever; //Task à qual está perto

    //The way the player is facing
    static bool up = false, down = false, left = false, right = false;

    switch (device) {
        case TIMER:
			//Verificar vitória
			victory=false; //ALTERAR PARA TRUE
			for(int index=0; index<n_tasks; index++){
				if(gameTasks[index]==NULL){
					continue;
				}else{
					victory=false;
					break;
				}
			}
			if(victory){
				//gameMenu=VICTORY;
				//Loadvictory();
				gameMenu=GAMEMAP;
				game_counter=20;
				LoadGameMap();
				break;
			}

			//Count down do jogo
			if(time_counter%60==0 && player->alive){
				game_counter--;
				erase_GameTimer();
				draw_GameTimer();
			}
			if(game_counter==0){
				gameMenu = FINAL; //Vai para defeat
			}

			//Change of rooms
			if (roomTransition()) {
				LoadPlay(room->currentRoom);
				break;
			}

			//Player Animation
			if(player->alive){
				if (time_counter % 7 == 0 && (keyboard_data == 0x11 || keyboard_data == 0x48 || keyboard_data == 0x1E ||keyboard_data == 0x4B))
					animate_player(player);
				if(time_counter % 7 == 0 && (keyboard_data == 0x1F ||keyboard_data == 0x50 || keyboard_data == 0x20 ||keyboard_data == 0x4D))
					animate_player(player);
			}
			//Move player
			move_player(player, up, down, left, right);

			//Projectile animation
			if (pr_exists) {
        		pr_exists=animate_projectile(playerProjectile);
        		if (!playerProjectile->exists){
					projectile_anim=true;
				}
      		}
			if(projectile_anim && time_counter%2==0){
				projectile_index++;
				erase_projectile(playerProjectile);
				explode_projectile(playerProjectile, projectile_index);
				if(projectile_index==3){
					erase_projectile(playerProjectile);
					canBlast=true;
					projectile_anim=false;
					projectile_index=0;
					playerProjectile->projectileImg=playerProjectile->projectileAnimations[projectile_index];
				}
			}

			//Move opponents
			bool found=false;
			for (int index = 0; index < n_opponents; index++) {
				if(gameOpponents[index]==NULL) continue;
       	 		else if(gameOpponents[index]->opponentRoom==room->currentRoom){
					found=true;
					if (time_counter % 7 == 0 && gameOpponents[index]->isMoving) //Update opponent animation
						animate_opponent(gameOpponents[index]);
						
					move_opponent(gameOpponents[index]); //Move opponent
				}
        		else if(found) break;
			}
			

			//Player collision with opponent
			if(player->alive){
				opponent_index=player_opponent_collision(player);
				if (opponent_index!=-1) {
					player->alive=false;
					gameOpponents[opponent_index]->isMoving=false;
				}
			}

			//Player Death
			if (!player->alive && !opponent_anim) {
				erase_opponent(gameOpponents[opponent_index]);
				if(player->direction==UP){
					gameOpponents[opponent_index]->direction=DOWN;
				}
				else if(player->direction==DOWN){
					gameOpponents[opponent_index]->direction=UP;
				}
				else if(player->direction==RIGHT){
					gameOpponents[opponent_index]->direction=LEFT;
				}
				else if(player->direction==LEFT){
					gameOpponents[opponent_index]->direction=RIGHT;
				}
				draw_opponent(gameOpponents[opponent_index]);
				opponent_anim=true;
			}
			//Death Animation
			if(opponent_anim && time_counter%(5+15*opponent_anim_index)==0){
				opponent_anim_index++;
				erase_opponent(gameOpponents[opponent_index]);
				if(opponent_anim_index==2){
					erase_player(player);
				}
				opponent_atack(gameOpponents[opponent_index], opponent_anim_index);
				if(opponent_anim_index == 3){
					gameMenu=FINAL;
				}
			}

			if (gameMenu == DEFEAT) {
				//Draw lost menu
			}
      		break;

    case KEYBOARD:
        //PAUSE MENU
        if (keyboard_data == ESC_KEY && player->alive) {
            gameMenu = PAUSE;
            //LoadRtc();
            //LoadPauseMenu();
            break;
        }

        //SHOOT
        if (keyboard_data == SPACEBAR_KEY && player->alive && canBlast/*Pra já não perde balas*/) {
			playerProjectile = blast(player);
			pr_exists=true;
			canBlast=false;
        }

        //Ativar tasks
        if (keyboard_data == E_KEY){
			int index=-1;
			if(player->direction==RIGHT) index=near_task(player->x+player->playerImg.width,player->y);	
			else if(player->direction==DOWN) index=near_task(player->x,player->y+player->playerImg.height);
			else index=near_task(player->x,player->y);
			if(index!=-1){
				switch(gameTasks[index]->taskType){
					case ICE:
						gameMenu=ICETASK;
						break;
					case SHIP:
						gameMenu=SHIPTASK;
						break;
					//Resto das tasks
					default:
						break;
				}
				LoadTask(index);
			}
        }

		//Ver mapa
		if (keyboard_data == M_KEY && player->alive) {
        	gameMenu = GAMEMAP;
        	LoadGameMap();
      	}

        //UPDATE MOVING DIRECTION
		if(player->alive)
        	change_direction(player, &up, &down, &left, &right);
        break;

    case MOUSE: //Não fazer nada ou clicar no map e pode vê-lo
        break;
    case RTC:
        break;
  }
}

//Interrupt handler das tasks

//Verificar se o player está na zona de transição de sala
bool roomTransition(){
	switch (room->currentRoom) {
		case CAFETERIA:
			if(player->direction==RIGHT && player->x>=722 && player->y > 275 && player->y < 305){room->currentRoom=HALLWAY1; return true;}
			else if(player->direction==LEFT && player->x <=2 && player->y >= 270 && player->y <= 310){room->currentRoom=HALLWAY2; return true;}
			break;
		case HALLWAY1:
			if(player->direction==LEFT && player->x <=2 && player->y >= 168 && player->y <= 235){room->currentRoom=CAFETERIA; return true;}
			else if(player->direction==RIGHT && player->x >=719 && player->y>=300 && player->y<=450){room->currentRoom=NAVIGATION; return true;}
			else if(player->direction==UP && player->x>=87 && player->x <=205 && player->y<=2){room->currentRoom=WEAPONS; return true;}
			else if(player->direction==DOWN && player->x>=400 && player->x <=520 && player->y>=524){room->currentRoom=ADMIN; return true;}
			break;
		case ADMIN:
			if(player->direction==UP && player->x>=57 && player->x <=138 && player->y<=1){room->currentRoom=HALLWAY1; return true;}
			break;
		case WEAPONS:
			if(player->direction==DOWN && player->x>=139 && player->x <=224 && player->y>=524){room->currentRoom=HALLWAY1; return true;}
			break;
		case NAVIGATION:
			if(player->direction==LEFT && player->x <=1 && player->y >= 276 && player->y <= 352){room->currentRoom=HALLWAY1; return true;}
			break;
		case HALLWAY2:
			if(player->direction==LEFT && player->x <=2 && player->y >= 300 && player->y <= 440){room->currentRoom=UPPERENG; return true;}
			else if(player->direction==RIGHT && player->x >=720 && player->y>=300 && player->y<=450){room->currentRoom=CAFETERIA; return true;}
			else if(player->direction==UP && player->x>=100 && player->x <=226 && player->y<=27){room->currentRoom=MEDBAY; return true;}
			else if(player->direction==DOWN && player->x>=387 && player->x <=512 && player->y>=524){room->currentRoom=ELETRICAL; return true;}
			break;
		case MEDBAY:
			if(player->direction==DOWN && player->x>=174 && player->x <=266 && player->y>=524){room->currentRoom=HALLWAY2; return true;}
			break;
		case ELETRICAL:
			if(player->direction==UP && player->x>=383 && player->x <=477 && player->y<=41){room->currentRoom=HALLWAY2; return true;}
			break;
		case UPPERENG:
			if(player->direction==RIGHT && player->x>=720 && player->y>= 230 && player->y <= 300){room->currentRoom=HALLWAY2; return true;}
			else if(player->direction==DOWN && player->x>=285 && player->x <=375 && player->y>=524){room->currentRoom=HALLWAY3; return true;}
			break;
		case HALLWAY3:
			if(player->direction==LEFT && player->x <=2 && player->y >= 275 && player->y <= 405){room->currentRoom=REACTOR; return true;}
			else if(player->direction==RIGHT && player->x >=722 && player->y>=265 && player->y<=405){room->currentRoom=SECURITY; return true;}
			else if(player->direction==UP && player->x>=259 && player->x <=480 && player->y<=42){room->currentRoom=UPPERENG; return true;}
			else if(player->direction==DOWN && player->x>=260 && player->x <=495 && player->y>=524){room->currentRoom=LOWERENG; return true;}
			break;
		case LOWERENG:
			if(player->direction==UP && player->x>=350 && player->x <=485 && player->y<=42){room->currentRoom=HALLWAY3; return true;}
			break;
		case REACTOR:
			if(player->direction==RIGHT && player->x>=720 && player->y > 290 && player->y < 325){room->currentRoom=HALLWAY3; return true;}
			break;
		case SECURITY:
			if(player->direction==LEFT && player->x <=2 && player->y >= 310 && player->y <= 380){room->currentRoom=HALLWAY3; return true;}
			break;
		//o resto dos rooms
		default:
			break;
	}
	return false;
}
