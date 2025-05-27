#include <Kernel.hpp>

Kernel::Kernel(std::unique_ptr<Scheduler> scheduler, std::unique_ptr<Logger> logger)
    :   m_scheduler(std::move(scheduler)),
        m_dispatcher(std::make_unique<Dispatcher>()),
        m_logger(logger ? std::move(logger) : std::make_unique<ConsoleLogger>("Kernel", true)) {}

Kernel::~Kernel(){}

void Kernel::addTask(std::unique_ptr<TaskControlBlock> task){
    m_taskList.push_back(std::move(task));
}

void Kernel::run(u_int8_t simulation_time){
    for (u_int8_t tick = 0; tick < simulation_time; ++tick){
        // Create vector to hold tasks that are ready for execution
        std::vector<TaskControlBlock*> temp_tasks;

        // Populate temp_tasks with tasks that are ready at the current tick
        for (auto& task : m_taskList){
            temp_tasks.push_back(task.get());
        }

        // Select the next task to run using the scheduler
        auto next_task = m_scheduler->select_next_task(temp_tasks, tick);
        if(next_task && next_task->getStatus() == TaskStatus::READY && next_task->getArrivalTime() <= tick){
            next_task->execute(tick);
            tick += next_task->getComputationTime() - 1;    // Simulating the execution time of the task
        } else {
            // If no task is ready, log the idle state
            m_logger->log("Tick " + std::to_string(tick));
        }
    }

    // Log the final state of all tasks
    m_logger->log("\n\n----------------- TASK STATISTICS -----------------");
    for (const auto& task : m_taskList) {
        m_logger->log(task->get_stats().str());
    }
    m_logger->log("\n----------------- END OF TASK STATISTICS -----------------");
}
