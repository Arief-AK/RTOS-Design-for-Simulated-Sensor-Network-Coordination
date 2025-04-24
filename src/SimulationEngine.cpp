#include <SimulationEngine.hpp>

SimulationEngine::SimulationEngine(std::unique_ptr<TaskController> task_controller, std::unique_ptr<Scheduler> scheduler, int max_ticks)
    : m_task_controller(std::move(task_controller)), m_scheduler(std::move(scheduler)), m_current_time(0), m_max_ticks(max_ticks){
        m_resource = std::make_shared<Resource>(1);
    }

SimulationEngine::~SimulationEngine(){
    m_task_controller.reset();
}

void SimulationEngine::run(){
    std::cout << "********* " << m_scheduler->getName() << " Simulation Engine Started *********\n";

    while (m_current_time < m_max_ticks){
        std::cout << "[Tick: " << m_current_time << "]\n";

        // Organise tasks based on current time
        m_task_controller->organiseTasks(m_current_time);

        // Initialise tasks from task controller
        m_task_list = m_task_controller->getReadyTasks();

        // Select task
        auto task = m_scheduler->selectTask(m_task_list);
        
        // Update task status
        if(task && task->status != TaskStatus::BLOCKED){
            std::cout << "Selected Task ID: " << task->task_id << "\n";

            if(task->requires_resource){
                auto acquired = m_resource->lock(std::make_shared<TaskControlBlock>(*task));
                if(!acquired){
                    std::cout << "Task ID: " << task->task_id << " blocked due to resource unavailability.\n";
                    continue; // Skip to next tick
                }
            }

            if(task->start_time == -1){
                task->start_time = m_current_time;
                task->status = TaskStatus::RUNNING;
            }

            task->remaining_time--;

            if(task->remaining_time <= 0){
                task->status = TaskStatus::COMPLETED;
                task->finish_time = m_current_time + 1; // Finish time is the next tick

                m_task_controller->addTask(std::make_shared<TaskControlBlock>(*task));
                std::cout << "Task ID: " << task->task_id << " completed at tick: " << m_current_time << "\n";

                
                if(task->requires_resource && m_resource->isLockedBy(std::make_shared<TaskControlBlock>(*task))){
                    m_resource->unlock(std::make_shared<TaskControlBlock>(*task));
                    std::cout << "Task ID: " << task->task_id << " released the resource.\n";
                }
            }else{
                task->status = TaskStatus::READY;
            }
        }else{
            std::cout << "No task selected for execution at tick: " << m_current_time << "\n";
        }

        // Update current time
        m_current_time++;
    }

    std::cout << "********* " << m_scheduler->getName() << " Simulation Engine Finished *********\n";
}

int SimulationEngine::getCurrentTime() const{
    return m_current_time;
}

void SimulationEngine::printStatistics() const{
    std::cout << "********* Task Statistics *********\n";

    // Initialise timing variables
    auto total_turnaround = 0;
    auto total_waiting_time = 0;
    auto completed_tasks = 0;
    auto cpu_time = 0;

    // Iterate through tasks and calculate statistics
    for(const auto& task : m_task_list){
        // Check for unfinished tasks
        if(task->finish_time == -1){
            std::cout << "Task ID: " << task->task_id << " is still running.\n";
            continue;
        }

        // Initialise interim timing variables
        int turnaround = task->finish_time - task->arrival_time;
        int waiting_time = turnaround - task->execution_time;

        // Update total timing variables
        total_turnaround += turnaround;
        total_waiting_time += waiting_time;
        cpu_time += task->execution_time;
        completed_tasks++;

        std::cout << "Task ID: " << task->task_id
            << "\n\t" << "Start Time: " << task->start_time
            << "\n\t" << "Finish Time: " << task->finish_time
            << "\n\t" << "Remaining Time: " << task->remaining_time
            << "\n\t" << "Turnaround Time: " << turnaround
            << "\n\t" << "Waiting Time: " << waiting_time;
        std::cout << "---------------------------------\n";
    }

    // Calculate and display averages
    auto avg_turnaround = completed_tasks > 0 ? static_cast<double>(total_turnaround) / completed_tasks : 0.0;
    auto avg_waiting_time = completed_tasks > 0 ? static_cast<double>(total_waiting_time) / completed_tasks : 0.0;
    auto cpu_utilization = static_cast<double>(cpu_time) / m_current_time * 100.0;

    // Print statistics
    std::cout << "Average Turnaround Time: " << avg_turnaround << "\n";
    std::cout << "Average Waiting Time: " << avg_waiting_time << "\n";
    std::cout << "CPU Utilization: " << cpu_utilization << "%\n";
    std::cout << "Task completion ratio: " << completed_tasks << "/" << m_task_list.size() << "\n";
}

std::vector<std::shared_ptr<TaskControlBlock>> SimulationEngine::getCompletedTasks() const{
    return m_task_controller->getCompletedTasks();
}
