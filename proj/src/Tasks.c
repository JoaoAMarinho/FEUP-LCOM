#include "Tasks.h"

extern uint16_t horizontal_res, vertical_res;

Task ** gameTasks;
int n_tasks=5;

//---------------------------------------------------------------------------------------------

void LoadTasks(){
    gameTasks = (Task **) malloc(5 * sizeof(Task *));
}

Task* create_task();
