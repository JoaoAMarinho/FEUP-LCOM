#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <math.h>

// Any header files included below this line should have been created by you

#include "Opponents.h"

//Include dos xpm das tasks
#include "Xpms/Tasks/TaskArrow.xpm"
#include "Xpms/Tasks/TaskWarning.xpm"

//Task names
typedef enum {T1,CARD,T3} Task_type; //Ajeitar dps

//Task States
//typedef enum {START, TRANSITION, END} Task1;

typedef struct {
    Room_number taskRoom;
    Task_type taskType;

    int x, y;
    xpm_image_t taskImg;

    xpm_image_t taskAnimations[5];
    unsigned int animationIndex;

} Task;


void LoadTasks();

Task* create_task(int x, int y, Task_type taskName, Room_number currentRoom);

void draw_current_tasks();

void draw_task_arrow(Task* task,xpm_image_t img);

void draw_warnings();

void draw_task_warning(Room_number room,xpm_image_t img);

//void erase_task_arrow(Task* task);

void finish_task(int index);

int near_task(int x_position, int y_position);

