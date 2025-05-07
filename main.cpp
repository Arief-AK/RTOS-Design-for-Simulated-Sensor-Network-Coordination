#include <iostream>
#include <Controller.hpp>

int main(int argc, char** argv){
    std::cout << "Hello, from RTOS_sensor_network!\n";

    // Initialise the controller
    auto controller = Controller("main_controller", 15);

    // Check for arguments
    if(argc > 1){
        std::string scheduler_type = argv[1];
        controller.createScheduler(controller.parseSchedulerType(scheduler_type));
    } else{
        std::cout << "No scheduler type provided. Defaulting to Priority scheduler." << std::endl;
        controller.createScheduler(SchedulerType::PRIORITY);
    }

    // Add tasks to the controller
    controller.addTask(1, TaskType::APERIODIC, 0, 6, 10, 3, 0); // T1
    controller.addTask(2, TaskType::APERIODIC, 0, 4, 10, 2, 1); // T2
    controller.addTask(3, TaskType::APERIODIC, 0, 3, 10, 1, 2); // T3
    controller.displayTasks();

    controller.runSimulation();
    controller.displayCompletedTasks();

    // // Initialise variables
    // TaskController task_controller;

    // // ID, Type, Period, Execution, Deadline, Priority, Arrival
    // auto task1 = std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 6, 10, 3, 0); // T1
    // task_controller.addTask(task1); // T1
    
    // auto task2 = std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 4, 10, 2, 1); // T2
    // task_controller.addTask(task2); // T2

    // auto task3 = std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 3, 10, 1, 2); // T3
    // task_controller.addTask(task3); // T3

    // // Display tasks
    // task_controller.displayTasks();

    // // Initialise the simulation engine
    // SimulationEngine engine(
    //     std::make_unique<TaskController>(task_controller),
    //     std::make_unique<PriorityScheduler>(),
    //     std::make_shared<ConsoleLogger>("Main_PrioritySchedulerTest"),
    //     15);

    // // Run the simulation
    // engine.run();
    // auto completed_tasks = engine.getCompletedTasks();
    
    return 0;
}
