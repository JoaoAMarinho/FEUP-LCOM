#include "Game.h"

//Device global variables
//Timer
unsigned int time_counter=0;
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
//extern Date * date;
Menu gameMenu = MAIN;

//static Lever * leverBeingUsed;
//static xpm_image_t LeverPositions[7];
//static xpm_image_t LeverColors[2];
//static xpm_image_t CleanLever;

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
    //Other loads (tasks and other things)
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

  //free(room);

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
        //case LEVER:
        //    LeverInterruptHandler(device);
        //    break;
        case PAUSE:
            Pause_ih(device);
            break;
        case TRANSITION:             //Transição entre rooms a mostrar qual está (black screen com o nome da sala)
        //    Transition_ih(device);
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
    //static Bullet * playerBullet;
    //static bool bulletOnMap = false;
    //static int checkLever; //Task à qual está perto

    //The way the player is facing
    static bool up = false, down = false, left = false, right = false;

    switch (device) {
        case TIMER:
			if (!player->alive) {
				gameMenu = DEFEAT;
				//Draw lost menu
				break;
			}

			//Verificar se está na posição de troca de sala
			if (roomTransition()) {
				LoadPlay(room->currentRoom);
				break;
			}
			/*
			Remover tasks do vetor de tasks
			for (unsigned int i = 0; i < level->numLevers; i++) {
				if (level->leverList[i]->active && level->doorList[i]->active) {
				if (level->levelnum == 1) {
					remove_door_background(level->doorList[i+1]);
					clean_door(level->doorList[i+1]);
					level->doorList[i+1]->active = false;
				}
				remove_door_background(level->doorList[i]);
				clean_door(level->doorList[i]);
				level->doorList[i]->active = false;
				}
			}*/

			//Animação do player
			if (time_counter % 7 == 0 && (keyboard_data == 0x11 || keyboard_data == 0x48 || keyboard_data == 0x1E ||keyboard_data == 0x4B))
				animate_player(player);
			if(time_counter % 7 == 0 && (keyboard_data == 0x1F ||keyboard_data == 0x50 || keyboard_data == 0x20 ||keyboard_data == 0x4D))
				animate_player(player);

			//Change the player position
			move_player(player, up, down, left, right);

			//BULLET ANIMATION
			/*if (bulletOnMap) {
				bulletOnMap = bulletAnimation(playerBullet, level->enemyList, level->numEnemies);
				if (!playerBullet->active)
				clean_bullet(playerBullet);
			}*/

			//Verificar se está perto de uma task
			//checkLever = check_lever_position();


			//Coisas dos enimigos e tiros
			/*
			for (unsigned int i = 0; i < level->numEnemies; i++) {
				if (level->enemyList[i]->bullet->active && level->enemyList[i]->isStatic)
				enemyBulletAnimation(level->enemyList[i]->bullet, player);

				if (level->enemyList[i]->dead)
				continue;

				if (counter % 10 == 0 && !level->enemyList[i]->isReloading)
				enemy_idle_animation(level->enemyList[i]);
				else if (counter % 5 == 0 && level->enemyList[i]->isReloading)
				enemy_reload_animation(level->enemyList[i]);

				enemy_movement(level->enemyList[i]);

				if (counter == randomTimeShoot && level->enemyList[i]->isStatic) {
				enemy_shoot(level->enemyList[i]);
				enemyShoot = true;
				}
			}

			if (enemyShoot) {
				randomTimeShoot = counter + (rand() % 280) + 120;
				enemyShoot = false;
			}
			*/

			//CHECKS IF IT COLLIDED WITH ANY ENEMY
			/*
			if (check_collision_enemy(player, player->direction, level->enemyList, level->numEnemies)) {
				gameMenu = DEFEAT;
			}
			*/

			if (gameMenu == DEFEAT) {
				//Draw lost menu
			}
      		break;

    case KEYBOARD:
        //PAUSE MENU
        if (keyboard_data == ESC_KEY) {
            gameMenu = PAUSE;
            //LoadRtc();
            //LoadPauseMenu();
            break;
        }

        //SHOOT
        if (keyboard_data == 0xB9 /*Spacebar make-code*/ && player->numberProjectiles!=0) {
        //playerBullet = shoot(player);
        //draw_ammo_level();
        //bulletOnMap = true;
        }

        //Ativar tasks
        /*
        if (checkLever != 0 && keyboard_data == E) {
        enemyShoot = true;
        up = false; down = false; left = false; right = false;
        check_movement(player, &up, &down, &left, &right);
        gameState = LEVER;
        LoadLever(0,level->leverList[checkLever - 1]);
        }
        */

		//Ver mapa;

        //UPDATE MOVING DIRECTION
        change_direction(player, &up, &down, &left, &right);
        break;

    case MOUSE: //Não fazer nada ou clicar no map e pode vê-lo
        break;
    case RTC:
        break;
  }
}

//Interrupt handler das tasks

//Load dos rooms
void LoadPlay(Room_number currentRoom){
	static Room_number previousRoom = CAFETERIA;

	if (previousRoom!=currentRoom) {
		switch (currentRoom) {
			case CAFETERIA:
				if(previousRoom==HALLWAY1){
					player->direction = LEFT;
					player->x = 0;
					player->y = 204;
				}else if(previousRoom==HALLWAY2){
					//Player fica no lado esquerdo
					player->direction = RIGHT;
					//player->x = 105;
					//player->y = 650;
				}
				previousRoom = CAFETERIA;
				break;
			case HALLWAY1:
				if(previousRoom==CAFETERIA){
					player->direction = RIGHT;
					player->x = 0;
					player->y = 204;
				}else if(previousRoom==WEAPONS){
					//Player fica em cima
					player->direction = DOWN;
					//player->x = 105;
					//player->y = 650;
				}else if(previousRoom==NAVIGATION){
					//Player fica no lado direito
					player->direction = LEFT;
					//player->x = 105;
					//player->y = 650;
				}else if(previousRoom==ADMIN){
					//Player fica em baixo
					player->direction = UP;
					//player->x = 105;
					//player->y = 650;
				}
				previousRoom = HALLWAY1;
				break;
			case ADMIN:
				previousRoom = ADMIN;
				player->direction = DOWN;
				//player->x = 479;
				//player->y = 690;
				break;
			case WEAPONS:
				previousRoom = WEAPONS;
				player->direction = UP;
				//player->x = 479;
				//player->y = 690;
				break;
			case NAVIGATION:
				previousRoom = NAVIGATION;
				player->direction = RIGHT;
				//player->x = 479;
				//player->y = 690;
				break;
			case HALLWAY2:
				//If(){}
				previousRoom = HALLWAY2;
				player->direction = RIGHT;
				//player->x = 479;
				//player->y = 690;
				break;
			case MEDBAY:
				previousRoom = MEDBAY;
				player->direction = UP;
				//player->x = 479;
				//player->y = 690;
				break;
			case ELETRICAL:
				previousRoom = ELETRICAL;
				player->direction = DOWN;
				//player->x = 479;
				//player->y = 690;
				break;
			case UPPERENG:
				previousRoom = UPPERENG;
				player->direction = UP;
				//player->x = 479;
				//player->y = 690;
				break;
			case LOWERENG:
				previousRoom = LOWERENG;
				player->direction = DOWN;
				//player->x = 479;
				//player->y = 690;
				break;
			case REACTOR:
				previousRoom = REACTOR;
				player->direction = LEFT;
				//player->x = 479;
				//player->y = 690;
				break;
			//o resto dos rooms
			case END:
				gameMenu = VICTORY;
				free(player);
				//LoadWonMenu();
				break;
			default:
				break;
		}
		if (gameMenu == PLAYING) {
			room = load_room(currentRoom);
		}
 	}
  else { //Caso de pausa, ver o mapa ou fazer uma task ou início
   if(room==NULL) room = load_room(currentRoom);
    draw_room();
    draw_room_enemies();
    draw_player(player);
  }

  //scancode = 0x00;

}

//Verificar se o player está na zona de transição de sala
bool roomTransition(){
	switch (room->currentRoom) {
		case CAFETERIA:
			if(player->direction==RIGHT && player->x>=724 && player->y > 280 && player->y < 305){room->currentRoom=HALLWAY1; return true;}
			else if(player->direction==LEFT && player->x+player->img.width<50 && player->y>276 && player->y<300){room->currentRoom=HALLWAY2; return true;}
			break;
		case HALLWAY1:
			break;
		case ADMIN:
			break;
		case WEAPONS:
			break;
		case NAVIGATION:
			break;
		case HALLWAY2:
			break;
		case MEDBAY:
			break;
		case ELETRICAL:
			break;
		case UPPERENG:
			break;
		case LOWERENG:
			break;
		case REACTOR:
			break;
		//o resto dos rooms
		default:
			break;
	}
	return false;
}
