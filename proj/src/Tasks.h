#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <math.h>

// Any header files included below this line should have been created by you

#include "Opponents.h"

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

//Task names
typedef enum {ICE,SHIP,T3} Task_type; //Ajeitar dps

//Task States
typedef enum {START_STATE, TRANSITION_STATE, END_STATE} Ship_state;

typedef struct {
    Room_number taskRoom;
    Task_type taskType;

    int x, y;
    xpm_image_t taskImg;

    xpm_image_t taskAnimations[10];
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

