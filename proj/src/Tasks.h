#pragma once

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <math.h>

// Any header files included below this line should have been created by you

#include "Opponents.h"

//Task names
/**
 * @brief enum with all the task types
 * 
 */
typedef enum {ICE,SHIP,DOWNLOAD} Task_type;

//Task States
/**
 * @brief enum with the states of the task SHIP
 * 
 */
typedef enum {START_STATE, TRANSITION_STATE, END_STATE} Ship_state;

/**
 * @struct Task
 *
 * @brief Struct that represents the task information
 *
 * @var Task::taskRoom
 * Room where the task is located
 * @var Task::taskType
 * Type of task
 * @var Task::x
 * Task x position
 * @var Task::y
 * Task y position
 * @var Task::taskImg
 * Task current image
 * @var Task::taskAnimations
 * Task animations array
 * @var Task::animationIndex
 * Task animation index
 * @var Task::isFinished
 * Bool true task is finished, false otherwise
 */
typedef struct {
    Room_number taskRoom;
    Task_type taskType;

    int x, y;
    xpm_image_t taskImg;

    xpm_image_t taskAnimations[10];
    unsigned int animationIndex;
    bool isFinished;

} Task;

/**
 * @brief Allocates memory for all the existing task in the gameTasks array
 * 
 */
void LoadTasks();

/**
 * @brief Create a task object
 * 
 * @param x position of the task 
 *
 * @param y position of the task
 *
 * @param taskName task type
 *
 * @param currentRoom of the task
 *
 * @return Task* object created
 */
Task* create_task(int x, int y, Task_type taskName, Room_number currentRoom);

/**
 * @brief Draws the arrow pointing to each task in the room
 * 
 */
void draw_current_tasks();

/**
 * @brief Draws an arrow pointing to a specific task in the room
 * 
 * @param task task to be pointed 
 *
 * @param img with the arrow
 */
void draw_task_arrow(Task* task,xpm_image_t img);

/**
 * @brief Draws all the warnings to each task in the map menu
 * 
 */
void draw_warnings();

/**
 * @brief Draws a warning to a specific task in the map menu
 * 
 * @param room of the task
 *
 * @param img with the warning symbol
 */
void draw_task_warning(Room_number room,xpm_image_t img);

/**
 * @brief Finishes a task by putting its bool isFinished to true 
 * 
 * @param index of the task in the gameTasks array
 */
void finish_task(int index);

/**
 * @brief Checks if the player is near any task of the current room
 * 
 * @param x_position of the player
 *
 * @param y_position of the player 
 *
 * @return int -1 if not near any task, else index of the task it is near 
 */
int near_task(int x_position, int y_position);

