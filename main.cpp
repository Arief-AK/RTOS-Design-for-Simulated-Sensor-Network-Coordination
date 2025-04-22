#include <iostream>
#include <TaskController.hpp>
#include <RoundRobinScheduler.hpp>
#include <SimulationEngine.hpp>

int main(int, char**){
    std::cout << "Hello, from RTOS_sensor_network!\n";

    // Initialise variables
    TaskController task_controller;
    
    // Create sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 10, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::SPORADIC, 0, 2, 7, 2, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 1, 0, 3, 0));

    // Display tasks
    task_controller.displayTasks();

    // Intialise the simulation engine
    SimulationEngine engine(std::make_unique<TaskController>(task_controller), std::make_unique<RoundRobinScheduler>(2), 10);
    engine.run();
    
    return 0;
}
