#include <iostream>
#include <TaskController.hpp>
#include <RoundRobinScheduler.hpp>

int main(int, char**){
    std::cout << "Hello, from RTOS_sensor_network!\n";

    // Initialise variables
    TaskController task_controller;
    RoundRobinScheduler round_robin_scheduler(2);

    // Set the scheduler
    task_controller.setScheduler(std::make_shared<RoundRobinScheduler>(round_robin_scheduler));
    
    // Create sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 10, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::SPORADIC, 20, 5, 20, 2, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 30, 2, 30, 3, 0));

    // Display tasks
    task_controller.displayTasks();

    // Use the scheduler
    for (int i = 0; i < 5; i++){
        std::cout << "Scheduler iteration: " << i << "\n";
        
        // Get tasks
        auto tasks = task_controller.getTasks();
        auto task = round_robin_scheduler.selectTask(tasks, i);
        if (task != nullptr){
            std::cout << "Tick: " << i << " Executing Task ID: " << task->task_id << "\n";
            task->status = TaskStatus::RUNNING;
        } else {
            std::cout << "Tick: " << i << " No task selected";
        }
    }
    
    return 0;
}
