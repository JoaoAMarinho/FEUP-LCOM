#include "Tasks.h"

extern uint16_t horizontal_res, vertical_res;
extern Room * room;

Task ** gameTasks;
int n_tasks=1;

//---------------------------------------------------------------------------------------------

void LoadTasks(){
    gameTasks = (Task **) malloc(n_tasks * sizeof(Task *));

    //Tasks ordered by room
    gameTasks[0] = create_task(570,356,T1,ADMIN);
}

Task* create_task(int x, int y, Task_type taskName, Room_number currentRoom){
    Task* task= (Task*) malloc(sizeof(Task));

    task->x = x;
    task->y = y;

    task->taskRoom=currentRoom;
    task->taskType=taskName;
    task->animationIndex=0;

    xpm_image_t img;

    //Load task animation
    switch(task->taskType){
        case T1:
        /*
            //xpm_load(gameMap_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[0] = img;
            //xpm_load(opponent_anim1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[1] = img;
            //xpm_load(opponent_anim2_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[2] = img;
            //xpm_load(opponent_anim1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[3] = img;
        */
            break;
        case T2:
            break;
        case T3:
            break;
        //Other tasks;
        default:
            break;
    }

    free(&img);
    //task->taskImg = task->taskAnimations[0];

    return task;
}

void draw_task_arrow(Task* task,xpm_image_t img){
    uint32_t* arrowMap = (uint32_t*) img.bytes;
    for(int row = 0;row < img.height; row++){
        for(int column = 0;column < img.width; column++){
            if (*(arrowMap + column + row*img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(task->x+column-64,task->y+row,*(arrowMap + column + row*img.width));
        }
    }
}

/*
void erase_task_arrow(Task* task){
    uint32_t* backgroundMap=(uint32_t*)room->roomBackground.bytes;

    for(int i = task->x; i <= task->x+task->buttonClicked.width; i++) {
            for (int j = task->y; j < task->y + task->buttonClicked.height; j++) {
                if (i < (int)horizontal_res - 1 && j < (int)vertical_res - 1)
                    drawPixel(i,j,*(backgroundMap  + i + j * horizontal_res));
            }
        }
}
*/

void draw_current_tasks(){
    bool found=false;
    xpm_image_t img;
    xpm_load(taskArrow_xpm, XPM_8_8_8_8, &img);
    for (int index = 0; index < n_tasks; index++)
        if(gameTasks[index]==NULL) continue;
        else if(gameTasks[index]->taskRoom==room->currentRoom){draw_task_arrow(gameTasks[index],img); found=true;}
        else if(found) break;
}

void finish_task(int index) {
  if (gameTasks[index] != NULL){
        gameTasks[index] = NULL; 
    }
}
