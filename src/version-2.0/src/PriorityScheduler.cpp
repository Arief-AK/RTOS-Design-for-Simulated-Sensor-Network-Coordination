#include <PriorityScheduler.hpp>

TaskControlBlock* PriorityScheduler::select_next_task(std::vector<TaskControlBlock*>& task_list, uint8_t current_time) {
    TaskControlBlock* selected_task = nullptr;
    for (auto& task : task_list) {
        if (task->getStatus() == TaskStatus::READY && task->getArrivalTime() <= current_time) {
            if (!selected_task || task->getPriority() < selected_task->getPriority()) {
                selected_task = task;
            }
        }
    }
    return selected_task;
}

const std::string& PriorityScheduler::getName() const {
    return m_name;
}

std::unique_ptr<Scheduler> PriorityScheduler::clone() const {
    return std::make_unique<PriorityScheduler>(*this);
}