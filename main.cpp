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
    
    return 0;
}
