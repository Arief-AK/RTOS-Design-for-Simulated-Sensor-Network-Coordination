#include <PriorityScheduler.hpp>

PriorityScheduler::PriorityScheduler() : m_last_index(0) {}

PriorityScheduler::~PriorityScheduler(){}

TaskControlBlock *PriorityScheduler::selectTask(
    std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue){
    // Create a handler
    std::shared_ptr<TaskControlBlock> selected_task = nullptr;

    // Iterate through thee tasks
    for (const auto& task : ready_queue){
        if (task->status == TaskStatus::READY){
            if(!selected_task || task->priority < selected_task->priority){
                selected_task = task;
            }
        }
    }

    return selected_task.get();
}

std::string PriorityScheduler::getName() const{
    return "PriorityScheduler";
}
