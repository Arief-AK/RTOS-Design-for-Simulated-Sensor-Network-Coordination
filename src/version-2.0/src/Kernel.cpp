#include <Kernel.hpp>

Kernel::Kernel(std::unique_ptr<Scheduler> scheduler, std::unique_ptr<Logger> logger)
    :   m_scheduler(std::move(scheduler)),
        m_dispatcher(std::make_unique<Dispatcher>()),
        m_logger(logger ? std::move(logger) : std::make_unique<ConsoleLogger>("Kernel", true)),
        m_metricsCollector(std::make_unique<MetricsCollector>()),
        m_json_logger(std::make_unique<JSONLogger>("Kernel_JSON")) {}

Kernel::~Kernel(){}

void Kernel::addTask(std::unique_ptr<TaskControlBlock> task){
    m_taskList.push_back(std::move(task));
}

void Kernel::run(uint8_t simulation_time){
    for (uint8_t tick = 0; tick < simulation_time; ++tick){
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
            m_metricsCollector->recordTaskMetrics(*next_task);

            // Log to JSON
            m_json_logger->logStructure({
                {"tick", tick},
                {"task_id", next_task->getTaskId()},
                {"status", "RUNNING"},
                {"start_time", next_task->getStartTime()},
                {"finish_time", next_task->getFinishTime()},
                {"response_time", next_task->getResponseTime()},
                {"value", next_task->getValue()},
                {"criticality", (next_task->getCriticality() == TaskCriticality::HARD ? "HARD" :
                                 next_task->getCriticality() == TaskCriticality::FIRM ? "FIRM" : "SOFT")},
            });

            tick += next_task->getComputationTime() - 1;    // Simulating the execution time of the task
        } else {
            // If no task is ready, log the idle state
            m_logger->log("Tick " + std::to_string(tick));
        }
    }

    // Log the final state of all tasks
    m_logger->log("\n\n----------------- TASK STATISTICS -----------------");
    m_metricsCollector->generateSummaryReport();
    m_metricsCollector->exportJSONSummary();
    m_json_logger->flushToFile("Kernel_task_statistics");
    m_logger->log("\n----------------- END OF TASK STATISTICS -----------------");
}

void Kernel::runPreemptive(uint8_t simulation_time){
    // Initialise the current task
    m_current_task = nullptr;

    for (uint8_t tick = 0; tick < simulation_time; ++tick){
        // Update the ready queue
        _update_ready_queue(tick);

        // Select the next task based on scheduler's policy
        std::vector<TaskControlBlock*> ready_tasks;
        std::queue<TaskControlBlock*> temp_queue;

        while (!m_ready_queue.empty()){
            TaskControlBlock* task = m_ready_queue.front();
            m_ready_queue.pop();
            if(!task->isCompleted()){
                ready_tasks.push_back(task);
                temp_queue.push(task);
            }
        }
        m_ready_queue = temp_queue;

        // Get selected task from the scheduler
        auto selected = m_scheduler->select_next_task(ready_tasks, tick);
        
        // Check preemption: If higher priority task is selected
        if (selected && selected != m_current_task){
            if(m_current_task && !m_current_task->isCompleted()){
                m_current_task->setStatus(TaskStatus::READY);
                m_ready_queue.push(m_current_task);
            }
            m_current_task = selected;
        }

        // Execute tick of the current task
        if(m_current_task && !m_current_task->isCompleted()){
            m_current_task->setStatus(TaskStatus::RUNNING);
            m_current_task->run_tick(tick);

            m_logger->log("Tick " + std::to_string(tick) + ": Executing Task ID " + std::to_string(m_current_task->getTaskId()) + 
                          " (Remaining Time: " + std::to_string(m_current_task->getRemainingTime()) + ")");

            if(m_current_task->isCompleted()){
                m_current_task->setStatus(TaskStatus::COMPLETED);
                m_current_task = nullptr;
            } else {
                m_current_task->setStatus(TaskStatus::READY);
            }
        }else{
            m_logger->log("Tick " + std::to_string(tick) + ": Idle.");
        }
    }
}

void Kernel::_update_ready_queue(uint8_t tick){
    for (auto &task : m_taskList){
        if(task->getStatus() == TaskStatus::READY && task->getArrivalTime() == tick){
            m_ready_queue.push(task.get());
        }
    }
}
