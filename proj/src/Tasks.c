#include "Tasks.h"

//Include dos xpm das tasks
#include "Xpms/Tasks/TaskArrow.xpm"
#include "Xpms/Tasks/TaskWarning.xpm"

//Ice task xpms
#include "Xpms/Tasks/Ice-task/Icetask0.xpm"
#include "Xpms/Tasks/Ice-task/Icetask1.xpm"
#include "Xpms/Tasks/Ice-task/Icetask2.xpm"
#include "Xpms/Tasks/Ice-task/Icetask3.xpm"
#include "Xpms/Tasks/Ice-task/Icetask4.xpm"
#include "Xpms/Tasks/Ice-task/Icetask5.xpm"
#include "Xpms/Tasks/Ice-task/Icetask6.xpm"
#include "Xpms/Tasks/Ice-task/Icetask7.xpm"

//Ship task xpms
#include "Xpms/Tasks/Ship-task/Shiptask0.xpm"
#include "Xpms/Tasks/Ship-task/Shiptask1.xpm"
#include "Xpms/Tasks/Ship-task/Shiptask2.xpm"
#include "Xpms/Tasks/Ship-task/Shiptask3.xpm"
#include "Xpms/Tasks/Ship-task/Shiptask4.xpm"

//Download task xpms
#include "Xpms/Tasks/Download-task/Downloadtask0.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask1.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask2.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask3.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask4.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask5.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask6.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask7.xpm"
#include "Xpms/Tasks/Download-task/Downloadtask8.xpm"

//Sequence task xpms
#include "Xpms/Tasks/Sequence-task/Sequencetask0.xpm"
#include "Xpms/Tasks/Sequence-task/Sequencetask1.xpm"
#include "Xpms/Tasks/Sequence-task/Sequencetask2.xpm"
#include "Xpms/Tasks/Sequence-task/Sequencetask3.xpm"
#include "Xpms/Tasks/Sequence-task/Sequencetask4.xpm"

//Anomaly task xpms
#include "Xpms/Tasks/Anomaly-task/Anomalytask0.xpm"
#include "Xpms/Tasks/Anomaly-task/Anomalytask1.xpm"

//Power task xpms
#include "Xpms/Tasks/Power-task/Powertask0.xpm"
#include "Xpms/Tasks/Power-task/Powertask1.xpm"

extern uint16_t horizontal_res, vertical_res;
extern Room * room;

Task ** gameTasks;
int n_tasks=7;

//---------------------------------------------------------------------------------------------

void LoadTasks(){
    gameTasks = (Task **) malloc(n_tasks * sizeof(Task *));

    //Tasks ordered by room
    gameTasks[0] = create_task(442,307,ICE,WEAPONS,false);
    gameTasks[1] = create_task(590,146,SHIP,NAVIGATION,false);
    gameTasks[2] = create_task(123,54,DOWNLOAD,ELETRICAL,false);
    gameTasks[3] = create_task(169,95,SEQUENCE,REACTOR,false);
    gameTasks[4] = create_task(585,411,ANOMALY,MEDBAY,false);
    gameTasks[5] = create_task(283,62,DOWNLOAD,LOWERENG,true);
    gameTasks[6] = create_task(324,67,POWER,UPPERENG,false);
    
    //Loaded before
    for(int i=0; i<9; i++){
        gameTasks[5]->taskAnimations[i]=gameTasks[2]->taskAnimations[i];
    }
    gameTasks[5]->taskImg = gameTasks[5]->taskAnimations[0];
}

Task* create_task(int x, int y, Task_type taskName, Room_number currentRoom, bool alreadyLoaded){
    Task* task= (Task*) malloc(sizeof(Task));

    task->x = x;
    task->y = y;

    task->taskRoom=currentRoom;
    task->taskType=taskName;
    task->animationIndex=0;
    task->isFinished=false;

    if(alreadyLoaded) return task;

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
        case DOWNLOAD:
            xpm_load(Downloadtask0_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[0] = img;
            xpm_load(Downloadtask1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[1] = img;
            xpm_load(Downloadtask2_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[2] = img;
            xpm_load(Downloadtask3_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[3] = img;
            xpm_load(Downloadtask4_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[4] = img;
            xpm_load(Downloadtask5_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[5] = img;
            xpm_load(Downloadtask6_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[6] = img;
            xpm_load(Downloadtask7_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[7] = img;
            xpm_load(Downloadtask8_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[8] = img;
            break;
        case SEQUENCE:
            xpm_load(Sequencetask0_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[0] = img;
            xpm_load(Sequencetask1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[1] = img;
            xpm_load(Sequencetask2_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[2] = img;
            xpm_load(Sequencetask3_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[3] = img;
            xpm_load(Sequencetask4_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[4] = img;
            break;
        case ANOMALY:
            xpm_load(Anomalytask0_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[0] = img;
            xpm_load(Anomalytask1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[1] = img;
            break;
        case POWER:
            xpm_load(Powertask0_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[0] = img;
            xpm_load(Powertask1_xpm, XPM_8_8_8_8, &img);
            task->taskAnimations[1] = img;
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
