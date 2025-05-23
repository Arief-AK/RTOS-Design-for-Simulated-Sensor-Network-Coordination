#include <RMScheduler.hpp>
#include <ConsoleLogger.hpp>

int main() {
    // Initialize the logger
    ConsoleLogger logger("Main");
    logger.log("RTOS version 2.0 started.");

    std::vector<TaskControlBlock*> task_list;
    task_list.emplace_back(new TaskControlBlock(0, 0, 4, 20, TaskCriticality::HARD, 10));
    task_list.emplace_back(new TaskControlBlock(1, 5, 5, 25, TaskCriticality::FIRM, 7));
    task_list.emplace_back(new TaskControlBlock(2, 10, 6, 40, TaskCriticality::SOFT, 5));

    RMScheduler scheduler;

    for (u_int8_t tick = 0; tick < 15; ++tick){
        TaskControlBlock* next_task = scheduler.select_next_task(task_list, tick);
        if (next_task) {
            // Simulate task execution
            next_task->updateMetrics(tick);
            next_task->setStatus(TaskStatus::RUNNING);
            logger.log("Tick " + std::to_string(tick) + ": Running task " + std::to_string(next_task->getTaskId()) + " until tick " + std::to_string(tick + next_task->getComputationTime()));
        } else {
            logger.log("Tick " + std::to_string(tick) + ": No task to run.");
        }
    }
    logger.log("RTOS version 2.0 finished executing tasks.\n");
    
    // Print task statistics
    for (const auto& task : task_list) {
        logger.log(task->get_stats().str());
        delete task; // Clean up memory
    }

    return 0;
}