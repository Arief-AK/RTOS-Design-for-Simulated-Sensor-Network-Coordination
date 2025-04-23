#include <TaskController.hpp>

TaskController::TaskController(){
    m_pending_tasks.clear();
    m_ready_tasks.clear();
    m_completed_tasks.clear();
}

TaskController::~TaskController(){
    m_pending_tasks.clear();
    m_ready_tasks.clear();
    m_completed_tasks.clear();
}

void TaskController::addTask(std::shared_ptr<TaskControlBlock> task){
    // Check if task is null
    if(!task){
        std::cerr << "Error: Task is null!" << std::endl;
        return;
    }
    
    switch (task->status)
    {
    case TaskStatus::PENDING:
        m_pending_tasks.push_back(task);
        break;

    case TaskStatus::READY:
        m_ready_tasks.push_back(task);
        break;

    case TaskStatus::COMPLETED:
        m_completed_tasks.push_back(task);
        break;
    
    default:
        std::cerr << "Error: Invalid task status!" << std::endl;
        break;
    }
}

void TaskController::organiseTasks(int current_time){
    // Move tasks from pending to ready if current time is greater than or equal to the arrival time
    for (auto it = m_pending_tasks.begin(); it != m_pending_tasks.end();){
        if(current_time >= (*it)->arrival_time){
            (*it)->status = TaskStatus::READY;
            m_ready_tasks.push_back(*it);
            it = m_pending_tasks.erase(it);
        }else{
            ++it;
        }
    }
}

void TaskController::removeTask(int task_id){
    for (auto it = m_ready_tasks.begin(); it != m_ready_tasks.end(); ++it) {
        if ((*it)->task_id == task_id) {
            m_ready_tasks.erase(it);
            break;
        }
    }
}

void TaskController::displayTasks(){
    for (const auto& tasks: m_ready_tasks){
        std::cout << "Task ID: " << tasks->task_id << "\n";
        std::cout << "Task Type: " << static_cast<int>(tasks->task_type) << "\n";
        std::cout << "Period: " << tasks->period << "\n";
        std::cout << "Execution Time: " << tasks->execution_time << "\n";
        std::cout << "Deadline: " << tasks->deadline << "\n";
        std::cout << "Priority: " << tasks->priority << "\n";
        std::cout << "Arrival Time: " << tasks->arrival_time << "\n";
        std::cout << std::endl;
    }
}

std::vector<std::shared_ptr<TaskControlBlock>> TaskController::getPendingTasks(){
    return m_pending_tasks;
}

std::vector<std::shared_ptr<TaskControlBlock>> TaskController::getReadyTasks()
{
    return m_ready_tasks;
}

std::vector<std::shared_ptr<TaskControlBlock>> TaskController::getCompletedTasks(){
    return m_completed_tasks;
}
