#include <EDFScheduler.hpp>

EDFScheduler::EDFScheduler(){}

EDFScheduler::~EDFScheduler(){}

TaskControlBlock* EDFScheduler::selectTask(std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue){
    std::shared_ptr<TaskControlBlock> selected_task = nullptr;
    std::shared_ptr<TaskControlBlock> running_task = nullptr;
    int earliest_deadline = std::numeric_limits<int>::max();

    // Find running task if any
    for (const auto& task : ready_queue) {
        if (task->status == TaskStatus::RUNNING && task->remaining_time > 0) {
            running_task = task;
            selected_task = task;
            earliest_deadline = task->arrival_time + task->deadline;
            break;
        }
    }

    // Check for task with earlier deadline
    for (const auto& task : ready_queue) {
        if ((task->status == TaskStatus::READY || task->status == TaskStatus::RUNNING) && 
            task->remaining_time > 0) {
            int task_deadline = task->arrival_time + task->deadline;
            
            if (task_deadline < earliest_deadline || 
                (task_deadline == earliest_deadline && task->arrival_time < selected_task->arrival_time)) {
                // Preempt current task if any
                if (running_task) {
                    running_task->status = TaskStatus::READY;
                }
                selected_task = task;
                earliest_deadline = task_deadline;
            }
        }
    }

    // Set selected task to running
    if (selected_task) {
        selected_task->status = TaskStatus::RUNNING;
    }

    return selected_task ? selected_task.get() : nullptr;
}

std::string EDFScheduler::getName() const{
    return "EDFScheduler";
}
