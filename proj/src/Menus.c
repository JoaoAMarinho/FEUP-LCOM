#include "Menus.h"

extern unsigned int time_counter,game_counter;
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

GameTimer* gameTimer;
static Button ** mainButtons;
//static Button ** pauseButtons;
//static Button * continueButton;

//---------------------------------------------------------------------------------------------
//Interrupt handlers

void Main_ih(Device device){
    static Mouse_event * mouseEvent;
    bool playClicked = false, instructionsClicked = false, bestscoresClicked = false, exitClicked = false;
    static bool overPlay = false, overInstructions = false, overBestscores = false, overExit = false, overCalendar = false;

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
                    else if (overCalendar) {
                        overCalendar = false;
                        erase_button(mainButtons[4]);
                        mainButtons[4]->isMouseOver = false;
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
                case 'C':
                    if(overCalendar) {
                        draw_button(mainButtons[4]);
                        mouse_pack.delta_x=0;
                        mouse_pack.delta_y=0;
                        //Desenhar data
                        update_cursor(&mouse_pack);
                    }
                    else if (!overCalendar) {
                        overCalendar = true;
                        mainButtons[4]->isMouseOver = true;
                        draw_button(mainButtons[4]);
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

void GameMap_ih(Device device){
    switch (device) {
        case TIMER:
            if(time_counter%60==0){
				game_counter--;
                erase_GameTimer();
                draw_GameTimer();
			}
			if(game_counter==0){
				gameMenu = FINAL; //Vai para defeat
			}
            break;

        case KEYBOARD:
            if (keyboard_data == M_KEY || keyboard_data==ESC_KEY) {
        	    gameMenu = PLAYING;
        	    LoadPlay(room->currentRoom);
      	    }
            break;
        case MOUSE: //Nada
            break;
        case RTC:
            break;
    }
}

void Victory_ih(Device device){return;}

void Defeat_ih(Device device){return;}

//---------------------------------------------------------------------------------------------
//Load menus

void LoadMain(){
    xpm_load(MainBackGround, XPM_8_8_8_8, &current_background);
    //xpm_load(MainBackGround, XPM_8_8_8_8, &empty_background);

    cursor = create_cursor();

    mainButtons = (Button **) malloc(5 * sizeof(Button *));

    mainButtons[0] = createButton(PLAY_B,500,110);
    mainButtons[1] = createButton(INSTRUCTIONS_B,500,170);
    mainButtons[2] = createButton(BESTSCORES_B,500,230);
    mainButtons[3] = createButton(EXIT_B,500,290);
    mainButtons[4] = createButton(CALENDAR_B,27,524);
    

    
    add_button(mainButtons[0]);
    add_button(mainButtons[1]);
    add_button(mainButtons[2]);
    add_button(mainButtons[3]);
    add_button(mainButtons[4]);
    draw_Menu();
}


void LoadGameMap(){
    xpm_load(gameMap_xpm, XPM_8_8_8_8, &current_background);
    draw_Menu();
    draw_GameTimer();
}

void LoadGameTimer(){

    gameTimer=(GameTimer*) malloc(sizeof(GameTimer));

    xpm_image_t img;

    //Load numbers
    xpm_load(Number0_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[0] = img;
    xpm_load(Number1_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[1] = img;
    xpm_load(Number2_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[2] = img;
    xpm_load(Number3_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[3] = img;
    xpm_load(Number4_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[4] = img;
    xpm_load(Number5_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[5] = img;
    xpm_load(Number6_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[6] = img;
    xpm_load(Number7_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[7] = img;
    xpm_load(Number8_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[8] = img;
    xpm_load(Number9_xpm, XPM_8_8_8_8, &img);
    gameTimer->Numbers[9] = img;

    free(&img);

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
					player->x = 723;
					player->y = 290;
				}else if(previousRoom==HALLWAY2){
					player->direction = RIGHT;
					player->x = 8;
					player->y = 289;
				}
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
				break;
			case ADMIN:
				player->direction = DOWN;
				player->x = 107;
				player->y = 0;
				break;
			case WEAPONS:
				player->direction = UP;
				player->x = 179;
				player->y = 525;
				break;
			case NAVIGATION:
				player->direction = RIGHT;
				player->x = 0;
				player->y = 315;
				break;
			case HALLWAY2:
				if(previousRoom==CAFETERIA){
                    player->direction=LEFT;
                    player->x=713;
                    player->y=364;
                }else if(previousRoom==ELETRICAL){
                    player->direction = UP;
				    player->x = 449;
				    player->y = 515;
                }else if(previousRoom==UPPERENG){
                    player->direction = RIGHT;
				    player->x = 21;
				    player->y = 365;
                }else if(previousRoom==MEDBAY){
                    player->direction = DOWN;
				    player->x = 167;
				    player->y = 34;
                }
				break;
			case MEDBAY:
				player->direction = UP;
				player->x = 217;
				player->y = 518;
				break;
			case ELETRICAL:
				player->direction = DOWN;
				player->x = 421;
				player->y = 55;
				break;
			case UPPERENG:
                if(previousRoom==HALLWAY2){
                    player->direction=LEFT;
                    player->x=709;
                    player->y=275;
                }else if(previousRoom==HALLWAY3){
                    player->direction = UP;
			        player->x = 326;
				    player->y = 520;
                }
				break;
            case HALLWAY3:
                if(previousRoom==LOWERENG){
                    player->direction=UP;
                    player->x=368;
                    player->y=517;
                }else if(previousRoom==REACTOR){
                    player->direction = RIGHT;
				    player->x = 25;
				    player->y = 334;
                }else if(previousRoom==UPPERENG){
                    player->direction = DOWN;
				    player->x = 379;
				    player->y = 56;
                }else if(previousRoom==SECURITY){
                    player->direction = LEFT;
				    player->x = 709;
				    player->y = 349;
                }
                break;
			case LOWERENG:
				player->direction = DOWN;
				//player->x = 479;
				//player->y = 690;
				break;
			case REACTOR:
				player->direction = LEFT;
				player->x = 721;
				player->y = 311;
				break;
            case SECURITY:
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
            draw_room();
            draw_GameTimer();
            //Desenhar inimigos e tasks dessa room
            draw_current_opponents();
		}
 	}
    else { //Pausa, GameMap ou Task
        if(room==NULL) room = load_room(currentRoom); //Primeiro load
        draw_room();
        draw_GameTimer();
        //Desenhar inimigos e tasks dessa room
        draw_current_opponents();
        draw_player(player);
    }
    previousRoom = currentRoom;
}

//---------------------------------------------------------------------------------------------
//Draw menus

void draw_Menu() {

    uint32_t* map = (uint32_t*) current_background.bytes;
      
    for(int row = 0;row < current_background.height; row++){
        for(int column = 0;column < current_background.width; column++){
            drawPixel(column,row,*(map + column + row*current_background.width));
        }
    }
}

void draw_GameTimer(){
    unsigned int counter=game_counter;
    int n;
    if(counter>100){ //3 digits
        n=counter%10;
        draw_Number(423,4,n);
        counter=counter/10;
        n=counter%10;
        draw_Number(398,4,n);
        counter=counter/10;
        n=counter%10;
        draw_Number(373,4,n);
    }else if(counter>10){ //2 digits
        n=counter%10;
        draw_Number(412,4,n);
        counter=counter/10;
        n=counter%10;
        draw_Number(387,4,n);
    }else{ //1 digit
        n=counter%10;
        draw_Number(403,4,n);
    }
}

void erase_GameTimer(){//Limpar desde y=3 até y=34 e desde x=372 até 460 
    uint32_t* map = (uint32_t*) room->roomBackground.bytes;

    for(int i = 372; i < 460; i++) {
            for (int j = 3; j < 35; j++) {
                drawPixel(i,j,*(map  + i + j * horizontal_res));
            }
        }
}

void draw_Number(int x, int y, int n){
    uint32_t* map=(uint32_t*)gameTimer->Numbers[n].bytes;
    
    for(int row = 0;row < gameTimer->Numbers[n].height; row++){
        for(int column = 0;column < gameTimer->Numbers[n].width; column++){
            if (*(map + column + row*gameTimer->Numbers[n].width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(x+column,y+row,*(map + column + row*gameTimer->Numbers[n].width));
        }
    }
}

