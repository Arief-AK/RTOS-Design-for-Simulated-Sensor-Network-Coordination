#include <Kernel.hpp>
#include <RMScheduler.hpp>
#include <ConsoleLogger.hpp>

int main() {
    // Initialize the logger
    ConsoleLogger logger("Main");
    logger.log("RTOS version 2.0 started.");

    auto scheduler = std::make_unique<RMScheduler>();
    auto kernel = Kernel(std::move(scheduler));

    kernel.addTask(std::make_unique<TaskControlBlock>(0, 0, 4, 20, TaskCriticality::HARD, 10));
    kernel.addTask(std::make_unique<TaskControlBlock>(1, 5, 5, 25, TaskCriticality::FIRM, 7));
    kernel.addTask(std::make_unique<TaskControlBlock>(2, 10, 6, 40, TaskCriticality::SOFT, 5));

    kernel.run(50);
    
    logger.log("RTOS version 2.0 finished executing tasks.\n");
    return 0;
}