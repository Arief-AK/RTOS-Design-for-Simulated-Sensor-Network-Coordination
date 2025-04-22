#include <SimulationEngine.hpp>

SimulationEngine::SimulationEngine(std::unique_ptr<TaskController> task_controller, std::unique_ptr<Scheduler> scheduler, int max_ticks)
    : m_task_controller(std::move(task_controller)), m_scheduler(std::move(scheduler)), m_current_time(0), m_max_ticks(max_ticks)
{
    // Initialise tasks from task controller
    m_task_list = m_task_controller->getTasks();
}

SimulationEngine::~SimulationEngine()
{
    m_task_controller.reset();
}

void SimulationEngine::run(){
    std::cout << "********* " << m_scheduler->getName() << " Simulation Engine Started *********\n";
    // std::cout << "Using scheduler: " << m_scheduler->getName() << "\n";

    while (m_current_time < m_max_ticks){
        std::cout << "[Tick: " << m_current_time << "]\n";

        // Select task
        auto task = m_scheduler->selectTask(m_task_list, m_current_time);
        
        // Update task status
        if(task){
            std::cout << "Selected Task ID: " << task->task_id << "\n";

            // Simulate tick of execution
            task->remaining_time--;
            task->status = TaskStatus::RUNNING;

            if(task->remaining_time <= 0){
                task->status = TaskStatus::COMPLETED;
                std::cout << "Task ID: " << task->task_id << " completed at tick: " << m_current_time << "\n";
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
