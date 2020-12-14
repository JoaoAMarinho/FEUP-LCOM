#include "Menus.h"

extern uint16_t horizontal_res, vertical_res;
extern uint8_t keyboard_data;
extern struct packet mouse_pack;

extern Menu gameMenu;
extern Player * player;
extern Room * room;
extern Cursor * cursor;
//extern Date * date;

xpm_image_t current_background;
//xpm_image_t empty_background;

static Button ** mainButtons;
//static Button ** pauseButtons;
//static Button * continueButton;

//---------------------------------------------------------------------------------------------
//Interrupt handlers

void Main_ih(Device device){
    static Mouse_event * mouseEvent;
    bool playClicked = false, instructionsClicked = false, bestscoresClicked = false, exitClicked = false;
    static bool overPlay = false, overInstructions = false, overBestscores = false, overExit = false;

    switch (device) {
        case TIMER:
            switch(checkOverMain()){
                case 'N':
                    if (overPlay) {
                        overPlay = false;
                        erase_button(mainButtons[0]);
                        mainButtons[0]->isMouseOver = false;
                    }
                    else if (overInstructions) {
                        overInstructions = false;
                        erase_button(mainButtons[1]);
                        mainButtons[1]->isMouseOver = false;
                    }
                    else if (overBestscores) {
                        overBestscores = false;
                        erase_button(mainButtons[2]);
                        mainButtons[2]->isMouseOver = false;
                    }
                    else if (overExit) {
                        overExit = false;
                        erase_button(mainButtons[3]);
                        mainButtons[3]->isMouseOver = false;
                    }
                    break;
                case 'P':
                    if ( *mouseEvent == L_UP) {
                        playClicked = true;
                        break;
                    }else if(overPlay) {
                        draw_button(mainButtons[0]);
                        mouse_pack.delta_x=0;
                        mouse_pack.delta_y=0;
                        update_cursor(&mouse_pack);
                    }
                    else if (!overPlay) {
                        overPlay = true;
                        mainButtons[0]->isMouseOver = true;
                        draw_button(mainButtons[0]);
                    }
                    break;
                case 'I':
                    if (*mouseEvent == L_UP){
                        instructionsClicked = true;
                        break;
                    }else if(overInstructions) {
                        draw_button(mainButtons[1]);
                        mouse_pack.delta_x=0;
                        mouse_pack.delta_y=0;
                        update_cursor(&mouse_pack);
                    }
                    else if (!overInstructions) {
                        overInstructions = true;
                        mainButtons[1]->isMouseOver = true;
                        draw_button(mainButtons[1]);
                    }
                    break;
                case 'B':
                    if (*mouseEvent == L_UP) {
                        bestscoresClicked = true;
                        break;
                    }else if(overBestscores) {
                        draw_button(mainButtons[2]);
                        mouse_pack.delta_x=0;
                        mouse_pack.delta_y=0;
                        update_cursor(&mouse_pack);
                    }
                    else if (!overBestscores) {
                        overBestscores = true;
                        mainButtons[2]->isMouseOver = true;
                        draw_button(mainButtons[2]);
                    }
                    break;
                case 'E': 
                    if (*mouseEvent == L_UP) {
                        exitClicked = true;
                        break;
                    }else if(overExit) {
                        draw_button(mainButtons[3]);
                        mouse_pack.delta_x=0;
                        mouse_pack.delta_y=0;
                        update_cursor(&mouse_pack);
                    }
                    else if (!overExit) {
                        overExit = true;
                        mainButtons[3]->isMouseOver = true;
                        draw_button(mainButtons[3]);
                    }
                    break;
            }
            break;

        case KEYBOARD: //Nada
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor(&mouse_pack);
            break;
        case RTC:
            //clean_clock();
            //updateDateTime();
            //draw_clock();
            break;
    }

    if (exitClicked) // Sair
        gameMenu = FINAL;
    else if (playClicked) { // Começar o jogo
        gameMenu = PLAYING;
        player=create_player();
        //clean_clock();
        LoadPlay(CAFETERIA);
    }
    else if (instructionsClicked) { // Instruções
        gameMenu = INSTRUCTIONS;
        //clean_clock();
        //Desenhar instruçoes();
    }
    else if (bestscoresClicked) { // Mostrar best scores
        gameMenu = BESTSCORES;
        //desenhar bestscores();
    }
}

void Bestscores_ih(Device device){return;}

void Instructions_ih(Device device){return;}

void Pause_ih(Device device){return;}

void Victory_ih(Device device){return;}

void Defeat_ih(Device device){return;}

//---------------------------------------------------------------------------------------------
//Load menus

void LoadMain(){
    xpm_load(MainBackGround, XPM_8_8_8_8, &current_background);
    //xpm_load(MainBackGround, XPM_8_8_8_8, &empty_background);

    cursor = create_cursor();

    mainButtons = (Button **) malloc(4 * sizeof(Button *));

    mainButtons[0] = createButton(PLAY_B,500,110);
    mainButtons[1] = createButton(INSTRUCTIONS_B,500,170);
    mainButtons[2] = createButton(BESTSCORES_B,500,230);
    mainButtons[3] = createButton(EXIT_B,500,290);

    
    add_button(mainButtons[0]);
    add_button(mainButtons[1]);
    add_button(mainButtons[2]);
    add_button(mainButtons[3]);
    draw_Main();
}

//---------------------------------------------------------------------------------------------
//Load Rooms

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
					player->direction = DOWN;
					player->x = 146;
					player->y = 0;
				}else if(previousRoom==NAVIGATION){
					player->direction = LEFT;
					player->x = 725;
					player->y = 365;
				}else if(previousRoom==ADMIN){
					player->direction = UP;
					player->x = 462;
					player->y = 525;
				}
				previousRoom = HALLWAY1;
				break;
			case ADMIN:
				previousRoom = ADMIN;
				player->direction = DOWN;
				player->x = 107;
				player->y = 0;
				break;
			case WEAPONS:
				previousRoom = WEAPONS;
				player->direction = UP;
				player->x = 179;
				player->y = 525;
				break;
			case NAVIGATION:
				previousRoom = NAVIGATION;
				player->direction = RIGHT;
				player->x = 0;
				player->y = 315;
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
}

//---------------------------------------------------------------------------------------------
//Draw menus

void draw_Main() {

    uint32_t* map = (uint32_t*) current_background.bytes;
      
    for(int row = 0;row < current_background.height; row++){
        for(int column = 0;column < current_background.width; column++){
            drawPixel(column,row,*(map + column + row*current_background.width));
        }
    }
}
