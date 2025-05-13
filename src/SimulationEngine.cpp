#include <SimulationEngine.hpp>

SimulationEngine::SimulationEngine(
    std::string simulation_name,
    std::unique_ptr<TaskController> task_controller,
    std::unique_ptr<Scheduler> scheduler,
    std::shared_ptr<Logger> logger,
    int max_ticks
    ): m_simulation_name(simulation_name),
        m_task_controller(std::move(task_controller)),
        m_scheduler(std::move(scheduler)),
        m_logger(std::move(logger)),
        m_current_time(0),
        m_max_ticks(max_ticks)
    {
        auto logger_name = m_logger->getLoggerName();
        m_metrics_collector = std::make_unique<MetricsCollector>(m_simulation_name, std::make_shared<ConsoleLogger>(logger_name + "_MetricsCollector"), true, true);
        m_resource = std::make_shared<Resource>(1);
    }

SimulationEngine::~SimulationEngine(){
    m_task_controller.reset();
}

void SimulationEngine::run(){
    m_logger->log("\n\n********* " + m_scheduler->getName() + " Simulation Engine Started *********");

    // Initialise previous task
    TaskControlBlock* previous_task = nullptr;
    
    while (m_current_time < m_max_ticks){
        m_logger->log("Tick: " + std::to_string(m_current_time));

        // Organise tasks and get the ready list
        m_task_controller->organiseTasks(m_current_time);
        m_task_list = m_task_controller->getReadyTasks();

        // Select task
        auto task = m_scheduler->selectTask(m_task_list);
        
        // Update task status
        if(task && task->status != TaskStatus::BLOCKED){
            m_logger->log("Selected Task ID: " + std::to_string(task->task_id));

            // Context switch tracking
            if (previous_task && previous_task != task) {
                m_metrics_collector->incrementContextSwitchCount();
            }
            previous_task = task;

            // Resource acquisition
            if (task->requires_resource) {
                bool acquired = m_resource->lock(std::make_shared<TaskControlBlock>(*task));
                if (!acquired) {
                    m_logger->log("Task ID: " + std::to_string(task->task_id) + " blocked due to resource unavailability.");
                    task->status = TaskStatus::BLOCKED;
                    m_current_time++;
                    continue;
                }
            }

            // Set start time if first run
            if(task->start_time == -1){
                task->start_time = m_current_time;
                task->status = TaskStatus::RUNNING;
            }

            // Simulate task execution
            if(task->remaining_time > 0){
                task->remaining_time--;
                task->status = TaskStatus::RUNNING;
                
                // Check if task completes after this execution
                if(task->remaining_time == 0){
                    task->status = TaskStatus::COMPLETED;
                    task->finish_time = m_current_time + 1; // Complete at end of this tick

                    // Release resource if held
                    if(task->requires_resource && m_resource->isLockedBy(std::make_shared<TaskControlBlock>(*task))){
                        m_resource->unlock(std::make_shared<TaskControlBlock>(*task));
                        m_logger->log("Task ID: " + std::to_string(task->task_id) + " released the resource.");
                    }

                    // Re-register periodic tasks for next period
                    if(task->task_type == TaskType::PERIODIC) {
                        auto next_task = std::make_shared<TaskControlBlock>(
                            task->task_id,
                            task->task_type, 
                            task->period,
                            task->execution_time,
                            task->deadline,
                            task->priority,
                            task->arrival_time + task->period
                        );
                        if(next_task->arrival_time < m_max_ticks) {
                            next_task->status = TaskStatus::PENDING;
                            m_task_controller->addTask(next_task);
                            m_logger->log("Re-registered periodic task ID: " + std::to_string(task->task_id) + 
                                         " for next period at tick: " + std::to_string(next_task->arrival_time));
                        }
                    }

                    // Re-register task for metrics
                    m_task_controller->addTask(std::make_shared<TaskControlBlock>(*task));
                }
            }

        }else{
            m_logger->log("No task selected for execution at tick: " + std::to_string(m_current_time));
            m_metrics_collector->incrementCpuIdleTime();
            previous_task = nullptr;
        }

        m_current_time++;
    }

    // Analyse and print report
    m_metrics_collector->analyseTaskCompletion(m_task_controller->getCompletedTasks(), m_current_time);
    m_metrics_collector->printReport(true);

    m_logger->log("\n********* " + m_scheduler->getName() + " Simulation Engine Finished *********");
}

int SimulationEngine::getCurrentTime() const{
    return m_current_time;
}

int SimulationEngine::getContextSwitchCount() const{
    return m_metrics_collector->getContextSwitchCount();
}

int SimulationEngine::getCPUIdleTime() const{
    return m_metrics_collector->getCPUIdleTime();
}

std::vector<std::shared_ptr<TaskControlBlock>> SimulationEngine::getCompletedTasks() const{
    return m_task_controller->getCompletedTasks();
}
