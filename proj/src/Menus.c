#include "Menus.h"

extern unsigned int time_counter,game_counter;
extern uint16_t horizontal_res, vertical_res;
extern uint8_t keyboard_data;
extern struct packet mouse_pack;

extern Menu gameMenu;
extern Player * player;
extern Room * room;
extern Cursor * cursor;
extern Task ** gameTasks;
int task_index;
//extern Date * date;

xpm_image_t current_background;
//xpm_image_t background_obstacles;

GameTimer* gameTimer;
static Button ** mainButtons;
//static Button ** pauseButtons;
//static Button * continueButton;

//---------------------------------------------------------------------------------------------
//Menu interrupt handlers

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
            if (keyboard_data == M_KEY) {
        	    gameMenu = PLAYING;
        	    LoadPlay(room->currentRoom);
      	    }

            if(keyboard_data == ESC_KEY){
                //gameMenu=PAUSE;
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
//Tasks interrupt handlers
void Ice_ih(Device device){
    static Mouse_event * mouseEvent;
    static bool ice1_clicked = false, ice2_clicked = false, ice3_clicked = false, task_finished= false;

    switch (device) {
        case TIMER:
            if(time_counter%60==0 && game_counter!=0){
				game_counter--;
                erase_GameTimer();
                draw_GameTimer();
			}
			if(game_counter==0 && !task_finished){
				gameMenu = FINAL; //Vai para defeat
			}
            //Rato não pode passar por cima do timer !!!!!!!!!!!!!!!
            if(checkOverIce()==1 && !ice1_clicked){ //Over middle ice
                if ( *mouseEvent == L_UP) {
                    ice1_clicked = true;
                    if(!ice2_clicked && !ice3_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[1];
                    else if(ice2_clicked && !ice3_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[5];
                    else if(!ice2_clicked && ice3_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[6];
                    else if(ice2_clicked && ice3_clicked){
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[7];
                        task_finished=true;
                    }
                    LoadTask(task_index);
                }
            }else if(checkOverIce()==2 && !ice2_clicked){ //Over top ice
                if ( *mouseEvent == L_UP) {
                    ice2_clicked = true;
                    if(!ice1_clicked && !ice3_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[3];
                    else if(ice1_clicked && !ice3_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[5];
                    else if(!ice1_clicked && ice3_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[4];
                    else if(ice2_clicked && ice3_clicked){
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[7];
                        task_finished=true;
                    }
                    LoadTask(task_index);
                }
            }else if(checkOverIce()==3 && !ice3_clicked){ //Over bottom ice
                if ( *mouseEvent == L_UP) {
                    ice3_clicked = true;
                    if(!ice1_clicked && !ice2_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[2];
                    else if(ice1_clicked && !ice2_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[6];
                    else if(!ice1_clicked && ice2_clicked)
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[4];
                    else if(ice2_clicked && ice3_clicked){
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[7];
                        task_finished=true;
                    }
                    LoadTask(task_index);
                }
            }
            //End task
            if(task_finished)
                finish_task(task_index);
            break;

        case KEYBOARD:
            if (keyboard_data == E_KEY) {
        	    gameMenu = PLAYING;
        	    LoadPlay(room->currentRoom);
      	    }

            if(keyboard_data == ESC_KEY){
                //gameMenu=PAUSE;
            }
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor_without_draw(&mouse_pack);
            draw_GameTimer();
            draw_cursor();
            //update_cursor(&mouse_pack);
            break;
        case RTC:
            break;
    }
}

void Ship_ih(Device device){
    static bool task_finished= false;
    static Mouse_event* mouseEvent;

    switch (device) {
        case TIMER:
            if(time_counter%60==0 && game_counter!=0){
				game_counter--;
                erase_GameTimer();
                draw_GameTimer();
			}
			if(game_counter==0 && !task_finished){
				gameMenu = FINAL; //Vai para defeat
			}
            //Rato não pode passar por cima do timer !!!!!!!!!!!!!!!
            
            //End task
            if(task_finished)
                finish_task(task_index);
            
            break;

        case KEYBOARD:
            if (keyboard_data == E_KEY) {
        	    gameMenu = PLAYING;
        	    LoadPlay(room->currentRoom);
      	    }

            if(keyboard_data == ESC_KEY){
                //gameMenu=PAUSE;
            }
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor_without_draw(&mouse_pack);
            draw_GameTimer();
            draw_cursor();
            if(!task_finished)
                task_finished=ship_gesture_handler(mouseEvent);
            break;
        case RTC:
            break;
    } 
}

//---------------------------------------------------------------------------------------------
//Load menus

void LoadMain(){
    xpm_load(MainBackGround, XPM_8_8_8_8, &current_background);
    //xpm_load(MainBackGroundObstacles, XPM_8_8_8_8, &background_obstacles);

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
//Loadvictory
//Loaddefeat
//Load bestscores
//Load instructions

void LoadGameMap(){
    xpm_load(gameMap_xpm, XPM_8_8_8_8, &current_background);
    draw_Menu();
    draw_GameTimer();
    draw_warnings();
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
//Load tasks

void LoadTask(int index){
    //xpm_load(tasksObstacles_xpm, XPM_8_8_8_8, &background_obstacles);
    task_index=index;
    current_background=gameTasks[index]->taskImg;
    draw_Menu();
    draw_GameTimer();
    draw_cursor();
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
				player->x = 425;
				player->y = 60;
				break;
			case REACTOR:
				player->direction = LEFT;
				player->x = 721;
				player->y = 311;
				break;
            case SECURITY:
                player->direction=RIGHT;
                player->x=17;
                player->y=347;
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
            //Draw room opponents and tasks
            draw_current_opponents();
            draw_current_tasks();
		}
 	}
    else { //Pausa, GameMap ou Task
        if(room==NULL) room = load_room(currentRoom); //Primeiro load
        draw_room();
        draw_GameTimer();
        //Draw room opponents and tasks
        draw_current_opponents();
        draw_current_tasks();
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

//---------------------------------------------------------------------------------------------
//Gesture handlers

bool ship_gesture_handler(Mouse_event* mouseEvent){
    static int x_delta = 0, y_delta = 0;
    static Ship_state shipState = START_STATE;
    
    switch(shipState){
        case START_STATE:
            gameTasks[task_index]->animationIndex=0;
            gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[0];
            LoadTask(task_index);
            x_delta = 0;
            y_delta = 0;
            if (*mouseEvent == L_DOWN && cursor->x >=172 && cursor->y >= 199 && cursor->x <=205 && cursor->y <=221 ) {
                shipState = TRANSITION_STATE;
            }
            break;
        case TRANSITION_STATE:
            if(*mouseEvent!=L_DOWN){
                shipState=START_STATE;
            }
            else{ //Left button pressed
                if(gameTasks[task_index]->animationIndex==0){ //Didn't reach checkpoint1
                    if ((x_delta >= 88) && (fabs(y_delta/(float)x_delta) >= 1.5) && (fabs(y_delta/(float)x_delta) < 1.85) ) {
                        x_delta = 0;
                        y_delta = 0;
                        gameTasks[task_index]->animationIndex=1;
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[1];
                        LoadTask(task_index);
                    }else if ((mouse_pack.delta_x >= 0 && mouse_pack.delta_y <= 0) || (abs(mouse_pack.delta_x) <= 3 && abs(mouse_pack.delta_y) <= 3)){
                        x_delta += mouse_pack.delta_x;
                        y_delta += mouse_pack.delta_y;
                    }else {
                        shipState = START_STATE;
                    }
                }else if(gameTasks[task_index]->animationIndex==1){ //Didn't reach checkpoint2
                    if(cursor->y>374) shipState = START_STATE;
                    if ((x_delta >= 98) && (fabs(y_delta/(float)x_delta) >= 0.45) && (fabs(y_delta/(float)x_delta) < 0.9) ) {
                        x_delta = 0;
                        y_delta = 0;
                        gameTasks[task_index]->animationIndex=2;
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[2];
                        LoadTask(task_index);
                    }else if ((mouse_pack.delta_x >= 0 && mouse_pack.delta_y >= 0) || (abs(mouse_pack.delta_x) <= 3 && abs(mouse_pack.delta_y) <= 3)){
                        x_delta += mouse_pack.delta_x;
                        y_delta += mouse_pack.delta_y;
                    }else {
                        shipState = START_STATE;
                    }
                }else if(gameTasks[task_index]->animationIndex==2){ //Didn't reach checkpoint3
                    if(cursor->y<192) shipState = START_STATE;
                    if ((x_delta >= 90) && (fabs(y_delta/(float)x_delta) >= 0.45) && (fabs(y_delta/(float)x_delta) < 0.65) ) {
                        x_delta = 0;
                        y_delta = 0;
                        gameTasks[task_index]->animationIndex=3;
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[3];
                        LoadTask(task_index);
                    }else if ((mouse_pack.delta_x >= 0 && mouse_pack.delta_y >= 0) || (abs(mouse_pack.delta_x) <= 3 && abs(mouse_pack.delta_y) <= 3)){
                        x_delta += mouse_pack.delta_x;
                        y_delta += mouse_pack.delta_y;
                    }else {
                        shipState = START_STATE;
                    }
                }else if(gameTasks[task_index]->animationIndex==3){ //Didn't reach checkpoint4
                    if(cursor->y<192 || cursor->y > 401) shipState = START_STATE;
                    if ((x_delta >= 80) && (fabs(y_delta/(float)x_delta) >= 0.85) && (fabs(y_delta/(float)x_delta) <= 1.6) ) {
                        shipState=END_STATE;
                    }else if ((mouse_pack.delta_x >= 0 && mouse_pack.delta_y <= 0) || (abs(mouse_pack.delta_x) <= 3 && abs(mouse_pack.delta_y) <= 3)){
                        x_delta += mouse_pack.delta_x;
                        y_delta += mouse_pack.delta_y;
                    }else {
                        shipState = START_STATE;
                    }
                }
            }
            break;
        case END_STATE:
            gameTasks[task_index]->animationIndex=4;
            gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[4];
            LoadTask(task_index);
            return true;
            break;
        default:
            break; 
    }
    return false;
}


