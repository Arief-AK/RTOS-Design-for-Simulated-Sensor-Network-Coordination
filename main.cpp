#include <iostream>
#include <memory>
#include <vector>

#include <TaskController.hpp>

int main(int, char**){
    std::cout << "Hello, from RTOS_sensor_network!\n";

    // Initialise task controller
    TaskController task_controller;
    
    // Create sample tasks`
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 10, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::SPORADIC, 20, 5, 20, 2, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 30, 2, 30, 3, 0));

    // Display tasks
    task_controller.displayTasks();

    return 0;
}
