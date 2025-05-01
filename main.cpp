#include <iostream>
#include <TaskController.hpp>
#include <PriorityScheduler.hpp>
#include <RoundRobinScheduler.hpp>
#include <SimulationEngine.hpp>

int main(int, char**){
    std::cout << "Hello, from RTOS_sensor_network!\n";

    // Initialise variables
    TaskController task_controller;

    // ID, Type, Period, Execution, Deadline, Priority, Arrival
    auto task1 = std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 6, 10, 3, 0); // T1
    task_controller.addTask(task1); // T1
    
    auto task2 = std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 4, 10, 2, 1); // T2
    task_controller.addTask(task2); // T2

    auto task3 = std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 3, 10, 1, 2); // T3
    task_controller.addTask(task3); // T3

    // Display tasks
    task_controller.displayTasks();

    // Initialise the simulation engine
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        std::make_unique<PriorityScheduler>(),
        std::make_shared<ConsoleLogger>("Main_PrioritySchedulerTest"),
        15);

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();
    
    return 0;
}
