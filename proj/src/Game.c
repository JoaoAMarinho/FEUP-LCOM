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
static Player * player;
//extern Room * room;
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
    //static int checkLever;

    //The way the player is facing
    static bool up = false, down = false, left = false, right = false;

    switch (device) {
        case TIMER:
        if (!player->alive) {
            gameMenu = DEFEAT;
            up = false; down = false; left = false; right = false;
            move_player(player, up, down, left, right);
            //Draw lost menu
            break;
        }

      //Verificar se está na posição de troca de sala
      /*
      if (check_final_position() || scancode == 0x19 USED FOR DEBUGGING) {
        enemyShoot = true;
        up = false; down = false; left = false; right = false;
        check_movement(player, &up, &down, &left, &right);
        LoadSinglePLayer(level->levelnum + 1, true);
        break;
      }

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
      }
      */

      //Player animatioon
        if (time_counter % 10 == 0)
            animate_player(player);
      /*
      else if (counter % 5 == 0 && player->isReloading)
        reload_animation(player);  
      else if (counter % 3 == 0 && player->isShooting)
        shoot_animation(player); 
      */
      
      //BULLET ANIMATION
      /*if (bulletOnMap) {
        bulletOnMap = bulletAnimation(playerBullet, level->enemyList, level->numEnemies);
        if (!playerBullet->active)
          clean_bullet(playerBullet);
      }*/

      //Verificar se está perto de uma task
      //checkLever = check_lever_position();

      //Change the player position
        move_player(player, up, down, left, right);

      //ENEMY ANIMATION AND SHOOTS
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
        up = false; down = false; left = false; right = false;
        move_player(player, up, down, left, right);
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
        if (keyboard_data == 0xB9 /*Spacebar make-code*/ && player->numberBullets!=0) {
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

