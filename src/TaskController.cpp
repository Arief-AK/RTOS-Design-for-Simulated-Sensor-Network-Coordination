#include <TaskController.hpp>

TaskController::TaskController(){
    m_tasks.clear();
}

TaskController::~TaskController(){
    m_tasks.clear();
}

void TaskController::addTask(std::shared_ptr<TaskControlBlock> task){
    m_tasks.push_back(task);
}

void TaskController::removeTask(int task_id){
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if ((*it)->task_id == task_id) {
            m_tasks.erase(it);
            break;
        }
    }
}

void TaskController::displayTasks(){
    for (const auto& tasks: m_tasks){
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

std::vector<std::shared_ptr<TaskControlBlock>> TaskController::getTasks(){
    return m_tasks;
}
