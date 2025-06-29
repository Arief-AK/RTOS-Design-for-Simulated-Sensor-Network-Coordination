#include <RMScheduler.hpp>

TaskControlBlock *RMScheduler::select_next_task(std::vector<TaskControlBlock *> &task_list, uint8_t current_time){
    TaskControlBlock* selected_task = nullptr;

    for (auto& task : task_list){
        if(task->getStatus() == TaskStatus::READY && task->getArrivalTime() <= current_time){
            if(!selected_task || task->getRelDeadline() < selected_task->getRelDeadline()){
                selected_task = task;
            }
        }
    }
    
    return selected_task;
}

const std::string& RMScheduler::getName() const{
    return m_name;
}