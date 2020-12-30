#include "Tasks.h"

extern uint16_t horizontal_res, vertical_res;
extern Room * room;

Task ** gameTasks;
int n_tasks=2;

//---------------------------------------------------------------------------------------------

void LoadTasks(){
    gameTasks = (Task **) malloc(n_tasks * sizeof(Task *));

    //Tasks ordered by room
    gameTasks[0] = create_task(442,307,ICE,WEAPONS);
    gameTasks[1] = create_task(590,146,SHIP,NAVIGATION);
}

Task* create_task(int x, int y, Task_type taskName, Room_number currentRoom){
    Task* task= (Task*) malloc(sizeof(Task));

    task->x = x;
    task->y = y;

    task->taskRoom=currentRoom;
    task->taskType=taskName;
    task->animationIndex=0;
    task->isFinished=false;

    xpm_image_t img;

    //Load task animation
    switch(task->taskType){
        case ICE:
            xpm_load(Icetask0_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[0] = img;
            xpm_load(Icetask1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[1] = img;
            xpm_load(Icetask2_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[2] = img;
            xpm_load(Icetask3_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[3] = img;
            xpm_load(Icetask4_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[4] = img;
            xpm_load(Icetask5_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[5] = img;
            xpm_load(Icetask6_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[6] = img;
            xpm_load(Icetask7_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[7] = img;
            break;
        case SHIP:
            xpm_load(Shiptask0_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[0] = img;
            xpm_load(Shiptask1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[1] = img;
            xpm_load(Shiptask2_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[2] = img;
            xpm_load(Shiptask3_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[3] = img;
            xpm_load(Shiptask4_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[4] = img;
            break;
        //Other tasks;
        default:
            break;
    }

    free(&img);
    task->taskImg = task->taskAnimations[0];

    return task;
}

void draw_current_tasks(){
    bool found=false;
    xpm_image_t img;
    xpm_load(taskArrow_xpm, XPM_8_8_8_8, &img);
    for (int index = 0; index < n_tasks; index++)
        if(gameTasks[index]->isFinished) continue;
        else if(gameTasks[index]->taskRoom==room->currentRoom){draw_task_arrow(gameTasks[index],img); found=true;}
        else if(found) break;
}

void draw_task_arrow(Task* task,xpm_image_t img){
    uint32_t* arrowMap = (uint32_t*) img.bytes;
    for(int row = 0;row < img.height; row++){
        for(int column = 0;column < img.width; column++){
            if (*(arrowMap + column + row*img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(task->x+column-50,task->y+row,*(arrowMap + column + row*img.width));
        }
    }
}

void draw_warnings(){
    Room_number previous_room=END;
    xpm_image_t img;
    xpm_load(taskWarning_xpm, XPM_8_8_8_8, &img);
    for(int index=0; index < n_tasks; index++){
        if(gameTasks[index]->isFinished) continue;
        else if(previous_room==END){
            previous_room=gameTasks[index]->taskRoom;
            draw_task_warning(previous_room,img);
        }
        else if(previous_room!=gameTasks[index]->taskRoom){
            previous_room=gameTasks[index]->taskRoom;
            draw_task_warning(previous_room,img);
        }
    }
}

void draw_task_warning(Room_number room,xpm_image_t img){
    uint32_t* warningMap = (uint32_t*) img.bytes;
    int x,y;
    switch(room){
        case CAFETERIA:
            break;
        case HALLWAY1:
            break;
        case ADMIN:
            x=648;
            y=328;
            break;
        case WEAPONS:
            x=629;
            y=210;
            break;
        case NAVIGATION:
            x=739;
            y=274;
            break;
        case HALLWAY2:
            break;
        case MEDBAY:
            x=293;
            y=155;
            break;
        case ELETRICAL:
            x=305;
            y=303;
            break;
        case UPPERENG:
            x=199;
            y=123;
            break;
        case HALLWAY3:
            break;
        case LOWERENG:
            x=209;
            y=449;
            break;
        case REACTOR:
            x=92;
            y=269;
            break;
        case SECURITY:
            x=227;
            y=306;
            break;
        default: 
            break;
    }
    //Draw the symbol
    for(int row = 0;row < img.height; row++){
        for(int column = 0;column < img.width; column++){
            if (*(warningMap + column + row*img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(x+column,y+row,*(warningMap + column + row*img.width));
        }
    }
}

void finish_task(int index) {
  if (!gameTasks[index]->isFinished){
        gameTasks[index]->isFinished=true; 
    }
}

int near_task(int x_position, int y_position){
    bool found=false;
    for (int index = 0; index < n_tasks; index++){
        if(gameTasks[index]->isFinished) continue;
        else if(gameTasks[index]->taskRoom==room->currentRoom){
            found=true;
            if(sqrt((gameTasks[index]->x-x_position)*(gameTasks[index]->x-x_position)+(gameTasks[index]->y-y_position)*(gameTasks[index]->y-y_position))<=90){
                return index;
            }
        }
        else if(found) break;
    }
    return -1;
}
