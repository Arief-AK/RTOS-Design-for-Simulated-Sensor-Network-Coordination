#include <Controller.hpp>

CABuffer<int, BUFFER_SIZE> sensor_data_buffer;

int main() {
    // Initialize main variables
    Controller controller;
    ConsoleLogger logger("Main");
    logger.log("RTOS version 2.0 started.");

    // Run schedulers from the controller
    controller.RunRMScheduler();
    controller.RunEDFScheduler();
    controller.RunPriorityScheduler();

    // Run comparative experiment
    // controller.RunComparativeExperiment();

    logger.log("RTOS version 2.0 finished executing tasks.");
    return 0;
}