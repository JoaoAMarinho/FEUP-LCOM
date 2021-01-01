#include "Menus.h"

//Include das images dos menus
#include "Xpms/Main_Menu/BackGround.xpm"
#include "Xpms/BestScores.xpm"
#include "Xpms/Instructions.xpm"
#include "Xpms/Pause_Menu/PauseBackground.xpm"
#include "Xpms/GameMap.xpm"
#include "Xpms/Victory.xpm"
#include "Xpms/Defeat.xpm"

#include "Xpms/Numbers/Number0.xpm"
#include "Xpms/Numbers/Number1.xpm"
#include "Xpms/Numbers/Number2.xpm"
#include "Xpms/Numbers/Number3.xpm"
#include "Xpms/Numbers/Number4.xpm"
#include "Xpms/Numbers/Number5.xpm"
#include "Xpms/Numbers/Number6.xpm"
#include "Xpms/Numbers/Number7.xpm"
#include "Xpms/Numbers/Number8.xpm"
#include "Xpms/Numbers/Number9.xpm"

extern unsigned int time_counter,game_counter;
extern uint16_t horizontal_res, vertical_res;
extern uint8_t keyboard_data;
extern struct packet mouse_pack;

extern Menu gameMenu;
extern Player * player;
extern int pontos;
extern Room * room;
extern Cursor * cursor;
extern Task ** gameTasks;
int task_index;
extern Date * date;

xpm_image_t current_background;

GameTimer* gameTimer;
Score scores[5];
Button ** mainButtons;
Button ** pauseButtons;

void ResetGame();
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
                        eraseDate();
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
                        update_cursor(&mouse_pack);
                    }
                    else if (!overCalendar) {
                        overCalendar = true;
                        mainButtons[4]->isMouseOver = true;
                        draw_button(mainButtons[4]);
                        draw_Main_Date();
                    }
                    break;
            }
            break;

        case KEYBOARD:
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor(&mouse_pack);
            break;
        case RTC:
            updateDate();
            break;
    }

    if (exitClicked) // Sair
        gameMenu = FINAL;
    else if (playClicked) { // Começar o jogo
        gameMenu = PLAYING;
        keyboard_data = 0x00;
        player=create_player();
        LoadPlay(CAFETERIA,false);
    }
    else if (instructionsClicked) { // Instruções
        gameMenu = INSTRUCTIONS;
        LoadInstructions();
    }
    else if (bestscoresClicked) { // Mostrar best scores
        gameMenu = BESTSCORES;
        LoadBestsores();
    }
}

void Bestscores_ih(Device device){
    switch (device) {
        case TIMER:
            break;
        case KEYBOARD:
            if (keyboard_data == ESC_KEY) {
                gameMenu = MAIN;
                LoadRTC();
                LoadMain();
                draw_cursor();
            }
            break;
        case MOUSE:
            break;
        case RTC:
            break;
  }
}

void Instructions_ih(Device device){
    switch (device) {
        case TIMER:
            break;
        case KEYBOARD:
            if (keyboard_data == ESC_KEY) {
                gameMenu = MAIN;
                LoadRTC();
                LoadMain();
                draw_cursor();
            }
            break;
        case MOUSE:
            break;
        case RTC:
            break;
  }
}

void Pause_ih(Device device){
    static Mouse_event * mouseEvent;
    bool resumeClicked = false, menuClicked = false;
    static bool overResume = false, overMenu = false;

    switch (device) {
        case TIMER:
            switch(checkOverPause()){
                case 'N':
                    if (overResume) {
                        overResume = false;
                        erase_button(pauseButtons[0]);
                        pauseButtons[0]->isMouseOver = false;
                    }
                    else if (overMenu) {
                        overMenu = false;
                        erase_button(pauseButtons[1]);
                        pauseButtons[1]->isMouseOver = false;
                    }
                    break;
                case 'R':
                    if ( *mouseEvent == L_UP) {
                        resumeClicked = true;
                        break;
                    }else if(overResume) {
                        draw_button(pauseButtons[0]);
                        mouse_pack.delta_x=0;
                        mouse_pack.delta_y=0;
                        update_cursor(&mouse_pack);
                    }
                    else if (!overResume) {
                        overResume = true;
                        pauseButtons[0]->isMouseOver = true;
                        draw_button(pauseButtons[0]);
                    }
                    break;
                case 'M':
                    if (*mouseEvent == L_UP){
                        menuClicked = true;
                        break;
                    }else if(overMenu) {
                        draw_button(pauseButtons[1]);
                        mouse_pack.delta_x=0;
                        mouse_pack.delta_y=0;
                        update_cursor(&mouse_pack);
                    }
                    else if (!overMenu) {
                        overMenu = true;
                        pauseButtons[1]->isMouseOver = true;
                        draw_button(pauseButtons[1]);
                    }
                    break;
            }
            break;

        case KEYBOARD:
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor(&mouse_pack);
            break;
        case RTC:
            break;
    }

    if (resumeClicked) { // Começar o jogo
        overResume = false, overMenu = false;
        *mouseEvent=MOVE;
        cursor->x=400; cursor->y=300;
        gameMenu = PLAYING;
        LoadPlay(room->currentRoom,true);
        LoadPlay(room->currentRoom,false);
    }
    else if (menuClicked) { // Instruções
        overResume = false, overMenu = false;
        *mouseEvent=MOVE;
        cursor->x=400; cursor->y=300;
        gameMenu = MAIN;
        ResetGame();
        LoadRTC();
        LoadMain();
    }
}

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
            if (keyboard_data == M_KEY || keyboard_data == ESC_KEY) {
        	    gameMenu = PLAYING;
        	    LoadPlay(room->currentRoom,false);
      	    }
            break;
        case MOUSE: //Nada
            break;
        case RTC:
            break;
    }
}

void Victory_ih(Device device){
    switch (device) {
        case TIMER:
            break;
        case KEYBOARD:
            if (keyboard_data == ESC_KEY) {
                gameMenu = MAIN;
                LoadMain();
            }
            break;
        case MOUSE:
            break;
        case RTC:
            break;
    }
}

void Defeat_ih(Device device){

    switch (device) {
        case TIMER:
            break;
        case KEYBOARD:
            if (keyboard_data == ESC_KEY) {
                gameMenu = MAIN;
                //LoadRtc();
                LoadMain();
            }
            break;
        case MOUSE:
            break;
        case RTC:
            break;
    }
}

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
				gameMenu = DEFEAT;
			}

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
            if(task_finished){
                finish_task(task_index);
            }
            break;

        case KEYBOARD:
            if (keyboard_data==E_KEY || keyboard_data==ESC_KEY) {
        	    gameMenu = PLAYING;
                ice1_clicked = false; ice2_clicked = false; ice3_clicked = false; task_finished= false;
                *mouseEvent=MOVE;
                gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[0];
        	    LoadPlay(room->currentRoom,false);
      	    }
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor_without_draw(&mouse_pack);
            draw_GameTimer();
            draw_cursor();
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
				gameMenu = DEFEAT;
			}

            //End task
            if(task_finished)
                finish_task(task_index);
            
            break;

        case KEYBOARD:
            if (keyboard_data == E_KEY || keyboard_data == ESC_KEY) {
        	    gameMenu = PLAYING;
                task_finished= false;
                gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[0];
                ship_gesture_handler(mouseEvent,true);
                *mouseEvent=MOVE;
        	    LoadPlay(room->currentRoom,false);
      	    }
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor_without_draw(&mouse_pack);
            draw_GameTimer();
            draw_cursor();
            if(!task_finished)
                task_finished=ship_gesture_handler(mouseEvent,false);
            break;
        case RTC:
            break;
    } 
}

void Download_ih(Device device){
    static Mouse_event * mouseEvent;
    static bool download_started = false, task_finished= false;

    switch (device) {
        case TIMER:
            if(time_counter%60==0 && game_counter!=0){
				game_counter--;
                erase_GameTimer();
                draw_GameTimer();
                if(download_started && !task_finished){
                    if(gameTasks[task_index]->animationIndex==8){
                        task_finished=true;
                    }
                    else{
                        gameTasks[task_index]->animationIndex++;
                        gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[gameTasks[task_index]->animationIndex];
                        LoadTask(task_index);
                    }
                }
			}
			if(game_counter==0 && !task_finished){
				gameMenu = DEFEAT;
			}

            if(cursor->x>336 && cursor->x<459 && cursor->y>371 && cursor->y<406 && !download_started){
                if ( *mouseEvent == L_UP) {
                    download_started = true;
                    gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[1];
                    gameTasks[task_index]->animationIndex++;
                    LoadTask(task_index);
                }
            }

            //End task
            if(task_finished){
                finish_task(task_index);
            }
            break;

        case KEYBOARD:
            if (keyboard_data==E_KEY || keyboard_data==ESC_KEY) {
        	    gameMenu = PLAYING;
                download_started = false; task_finished= false;
                *mouseEvent=MOVE;
                gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[0];
                gameTasks[task_index]->animationIndex=0;
        	    LoadPlay(room->currentRoom,false);
      	    }
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor_without_draw(&mouse_pack);
            draw_GameTimer();
            draw_cursor();
            break;
        case RTC:
            break;
    }
}

void Sequence_ih(Device device){
    static Mouse_event * mouseEvent;
    static bool task_finished = false;

    switch (device) {
        case TIMER:
            if(time_counter%60==0 && game_counter!=0){
				game_counter--;
                erase_GameTimer();
                draw_GameTimer();
			}
			if(game_counter==0 && !task_finished){
				gameMenu = DEFEAT;
			}
            
            //End task
            if(task_finished){
                finish_task(task_index);
            }
            break;

        case KEYBOARD:
            if (keyboard_data==E_KEY || keyboard_data==ESC_KEY) {
        	    gameMenu = PLAYING;
                task_finished= false;
                *mouseEvent=MOVE;
                gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[0];
                sequence_gesture_handler(mouseEvent,true);
        	    LoadPlay(room->currentRoom,false);
      	    }
            break;
        case MOUSE:
            mouseEvent = get_mouse_event(&mouse_pack);
            update_cursor_without_draw(&mouse_pack);
            draw_GameTimer();
            draw_cursor();
            if(!task_finished)
                task_finished=sequence_gesture_handler(mouseEvent,false);
            break;
        case RTC:
            break;
    }
}

//---------------------------------------------------------------------------------------------
//Load menus

void LoadMain(){
    xpm_load(MainBackGround, XPM_8_8_8_8, &current_background);
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

void LoadBestsores(){
    xpm_load(BestScores_xpm, XPM_8_8_8_8, &current_background);

    draw_Menu();
    draw_BestScore(182,scores[0]);
    draw_BestScore(238,scores[1]);
    draw_BestScore(294,scores[2]);
    draw_BestScore(350,scores[3]);
    draw_BestScore(406,scores[4]);
}

void LoadInstructions(){
    xpm_image_t instructionsImg;
    xpm_load(Instructions_xpm, XPM_8_8_8_8, &instructionsImg);
    uint32_t* map = (uint32_t*) instructionsImg.bytes;
    for(int i = 0; i < instructionsImg.width; i++) {
        for (int j = 0; j < instructionsImg.height; j++) {
            drawPixel(i,j,*(map + i + j*horizontal_res));
        }
    }
}

void LoadPause(){
    xpm_load(PauseBackGround, XPM_8_8_8_8, &current_background);

    pauseButtons = (Button **) malloc(2 * sizeof(Button *));

    pauseButtons[0] = createButton(RESUME_B,510,146);
    pauseButtons[1] = createButton(MENU_B,510,236);

    add_button(pauseButtons[0]);
    add_button(pauseButtons[1]);
    draw_Menu();
}

void LoadGameMap(){
    xpm_load(gameMap_xpm, XPM_8_8_8_8, &current_background);
    draw_Menu();
    draw_GameTimer();
    draw_warnings();
}

void LoadVictory(){
    insertBestScores();
    xpm_load(Victory_xpm, XPM_8_8_8_8, &current_background);
    draw_Menu();
    draw_Score(445,419);
}

void LoadDefeat(){
    xpm_load(Defeat_xpm, XPM_8_8_8_8, &current_background);
    draw_Menu();
}

void LoadNumbers(){

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
    task_index=index;
    current_background=gameTasks[index]->taskImg;
    draw_Menu();
    draw_GameTimer();
    draw_cursor();
}

//---------------------------------------------------------------------------------------------
//Load Rooms

void LoadPlay(Room_number currentRoom, bool reset){
	static Room_number previousRoom = CAFETERIA;

    if(!reset){
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
        else { //Pause, GameMap or Task
            if(room==NULL) room = load_room(currentRoom); //Primeiro load
            draw_room();
            draw_GameTimer();
            //Draw room opponents and tasks
            draw_current_opponents();
            draw_current_tasks();
            draw_player(player);
        }
        previousRoom = currentRoom;
        keyboard_data = 0x00;

    }else{
        //CurrentRoom == CAFETERIA
        room = load_room(currentRoom);
        previousRoom=currentRoom;
    }
	
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

//---------------------------------------------------------------------------------------------
//Draw GameTimer

void draw_GameTimer(){
    unsigned int counter=game_counter;
    int n;
    if(counter>=100){ //3 digits
        n=counter%10;
        draw_Number(423,4,n);
        counter=counter/10;
        n=counter%10;
        draw_Number(398,4,n);
        counter=counter/10;
        n=counter%10;
        draw_Number(373,4,n);
    }else if(counter>=10){ //2 digits
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
//Draw Date

void draw_Main_Date(){
    draw_Number(100,538,date->day/10);
    draw_Number(121,538,date->day%10);
    draw_Symbol(140,538,0);
    draw_Number(148,538,date->month/10);
    draw_Number(164,538,date->month%10);
    draw_Symbol(182,538,0);
    draw_Number(191,538,2);
    draw_Number(212,538,0);
    draw_Number(232,538,date->year/10);
    draw_Number(254,538,date->year%10);
}

void eraseDate(){
    uint32_t* map = (uint32_t*) current_background.bytes;

    for(int i = 100; i < 290; i++) {
        for (int j = 538; j < 570; j++) {
            drawPixel(i,j,*(map  + i + j * horizontal_res));
        }
    }
}

void draw_Symbol(int x, int y, int n){
    uint32_t* map=(uint32_t*)date->Symbols[n].bytes;
    
    for(int row = 0;row < date->Symbols[n].height; row++){
        for(int column = 0;column < date->Symbols[n].width; column++){
            if (*(map + column + row*date->Symbols[n].width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(x+column,y+row,*(map + column + row*date->Symbols[n].width));
        }
    }
}

//---------------------------------------------------------------------------------------------
//Gesture handlers

bool ship_gesture_handler(Mouse_event* mouseEvent, bool reset){
    static int x_delta = 0, y_delta = 0;
    static Task_state shipState = START_STATE;

    if(reset){
        x_delta = 0, y_delta = 0;
        shipState = START_STATE;
        return false;
    }
    
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
            x_delta = 0;
            y_delta = 0;
            shipState=START_STATE;
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

bool sequence_gesture_handler(Mouse_event* mouseEvent,bool reset){
    static bool n1_clicked = false, n2_clicked = false, n3_clicked = false;
    static Task_state sequenceState = START_STATE;

    if(reset){
        n1_clicked = false; n2_clicked = false; n3_clicked = false;
        sequenceState = START_STATE;
        return false;
    }
    
    switch(sequenceState){
        case START_STATE:
            gameTasks[task_index]->animationIndex=0;
            gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[0];
            LoadTask(task_index);
            if (*mouseEvent == L_UP && cursor->x > 248 && cursor->y > 327 && cursor->x < 389 && cursor->y < 465 ) {
                sequenceState = TRANSITION_STATE;
                n1_clicked = true;
                gameTasks[task_index]->animationIndex=1;
                gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[1];
                LoadTask(task_index);
            }
            break;
        case TRANSITION_STATE:
            if(*mouseEvent==L_UP && !n2_clicked && !n3_clicked){ //Just clicked n1
                if(cursor->x > 404 && cursor->y>168 && cursor->x<544 && cursor->y<306){
                    gameTasks[task_index]->animationIndex=2;
                    gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[2];
                    LoadTask(task_index);
                    n2_clicked=true;
                }else{
                    n1_clicked=false;
                    sequenceState=START_STATE;
                }
            }else if(*mouseEvent==L_UP && !n3_clicked){
                if(cursor->x > 254 && cursor->y>170 && cursor->x<392 && cursor->y<307){
                    gameTasks[task_index]->animationIndex=3;
                    gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[3];
                    LoadTask(task_index);
                    n3_clicked=true;
                    sequenceState=END_STATE;
                }else{
                    n2_clicked=false;
                    n1_clicked=false;
                    sequenceState=START_STATE;
                }
            }
            break;
        case END_STATE:
            if(*mouseEvent==L_UP){
                if(cursor->x > 405 && cursor->y>324 && cursor->x<544 && cursor->y<462){
                    gameTasks[task_index]->animationIndex=4;
                    gameTasks[task_index]->taskImg=gameTasks[task_index]->taskAnimations[4];
                    LoadTask(task_index);
                    return true;
                }else sequenceState=START_STATE; 
            }
            break;
        default:
            break; 
    }
    return false;
}

//---------------------------------------------------------------------------------------------
//Scores
void insertBestScores(){
    Score x;
    x.points=pontos;
    x.data=(*date);
    for (int i = 0; i < 5; i++){
        if(scores[i].points < x.points){
            Score temp_score = scores[i];
            scores[i] = x;
            x = temp_score;
        }
    }
}

void draw_Score(int x, int y){
    if(pontos>=100){ //3 digits
        draw_Number(x,y,pontos/100);
        draw_Number(x+25,y,(pontos/10)%10);
        draw_Number(x+50,y,pontos%10);
    }else if(pontos>=10){ //2 digits
        draw_Number(x,y,(pontos/10)%10);
        draw_Number(x+25,y,pontos%10);
    }else{ //1 digit
        draw_Number(x,y,pontos%10);
    }
}

void draw_BestScore(int y, Score score){
    
    //Draw score
    draw_Number(536,y,score.points/100);
    draw_Number(557,y,(score.points/10)%10);
    draw_Number(578,y,score.points%10);

    //Draw date
    draw_Date(y,score.data);
    
}
void draw_Date(int y,Date date){
    draw_Number(164,y,date.day/10);
    draw_Number(186,y,date.day%10);
    draw_Symbol(212,y,0);
    draw_Number(224,y,date.month/10);
    draw_Number(246,y,date.month%10);
    draw_Symbol(272,y,0);
    draw_Number(284,y,2);
    draw_Number(306,y,0);
    draw_Number(328,y,date.year/10);
    draw_Number(350,y,date.year%10);
}
