#include "Rooms.h"

extern uint16_t horizontal_res, vertical_res;

Room * room;

//---------------------------------------------------------------------------------------------

Room * load_room(Room_number roomNumber){
    free(room);
    room = (Room *) malloc (sizeof(room));
    room->currentRoom=roomNumber;
    //CAFETERIA, HALLWAY1, ADMIN, WEAPONS, NAVIGATION, HALLWAY2, MEDBAY, ELETRICAL, UPPERENG, LOWERENG, REACTOR, SECURITY, END
    if(roomNumber==CAFETERIA){
        xpm_load(Cafeteria_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(CafeteriaObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);

        room->taksNumber = 0;    //número de tasks
        room->enemiesNumber = 0; //número de enimigos
        
        //Posição de troca de sala
        //level->finalPos = createFinalPos(832,64,6);
        
        //Criar as tarefas
        //level->leverList = (Lever **) malloc (level->numLevers * sizeof(Lever *));
        //level->leverList[0] = createLever(384,704);
        //add_lever_background(level->leverList[0]);

        //Criar enemys
        //level->enemyList = (Enemy **) malloc (level->numEnemies * sizeof(Enemy *));
        //level->enemyList[0] = createEnemy(40,100,DOWN,false);
        //level->enemyList[1] = createEnemy(775,50,DOWN,false);
    }
    else if(roomNumber==HALLWAY1){
        xpm_load(Hallway1_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(Hallway1Obstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);

        room->taksNumber = 0;    //número de tasks
        room->enemiesNumber = 0; //número de enimigos
    }
    else if(roomNumber==ADMIN){
        xpm_load(Admin_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(AdminObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);

        room->taksNumber = 0;    //número de tasks
        room->enemiesNumber = 0; //número de enimigos
    }
    else if(roomNumber==WEAPONS){
        xpm_load(Weapons_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(WeaponsObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);

        room->taksNumber = 0;    //número de tasks
        room->enemiesNumber = 0; //número de enimigos
    }
    else if(roomNumber==NAVIGATION){
        xpm_load(Navigation_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(NavigationObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);

        room->taksNumber = 0;    //número de tasks
        room->enemiesNumber = 0; //número de enimigos
    }
    else if(roomNumber==HALLWAY2){
        return NULL;
    }
    else if(roomNumber==MEDBAY){
        return NULL;
    }
    else if(roomNumber==ELETRICAL){
        return NULL;
    }
    else if(roomNumber==UPPERENG){
        return NULL;
    }
    else if(roomNumber==LOWERENG){
        return NULL;
    }
    else if(roomNumber==REACTOR){
        return NULL;
    }
    else if(roomNumber==SECURITY){
        return NULL;
    }

    //level->level_back = (uint32_t*) level->level_background.bytes;


    /*if (levelNum != 0)
        add_final_position_background(&level->finalPos->posfinalx, &level->finalPos->posfinaly, &level->finalPos->lado);*/

    draw_room();
    //Após desenhar room desenhar inimigos e possíveis tasks, terá que ser chamado fora desta função
    //draw_room_enemies();

    return room;
    
}

void draw_room(){
    uint32_t* map=(uint32_t*)room->roomBackground.bytes;
    
    for(int row = 0;row < room->roomBackground.height; row++){
        for(int column = 0;column < room->roomBackground.width; column++){
            if (*(map + column + row*room->roomBackground.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(column,row,*(map + column + row*room->roomBackground.width));
        }
    }
}

void delete_room(){
    if (room != NULL){
        free(room->roomBackground.bytes);
        free(room);
        room = NULL; 
    }
}

void draw_room_enemies(){
  for (int i = 0; i < room->enemiesNumber; i++)
    continue;
    //draw_enemy(room->enemyList[i]);
}

void draw_room_tasks(){
  for (int i = 0; i < room->taksNumber; i++)
    continue;
    //draw_task(room->leverList[i]);
}
