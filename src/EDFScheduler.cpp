#include <EDFScheduler.hpp>

EDFScheduler::EDFScheduler(){}

EDFScheduler::~EDFScheduler(){}

TaskControlBlock* EDFScheduler::selectTask(std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue){
    // Create a handler
    std::shared_ptr<TaskControlBlock> selected_task = nullptr;

    auto min_deadline = std::numeric_limits<int>::max();

    // Iterate through the tasks
    for (const auto& task: ready_queue){
        if(task->status == TaskStatus::READY){
            auto absolute_deadline = task->arrival_time + task->deadline;
            if(absolute_deadline < min_deadline){
                min_deadline = absolute_deadline;
                selected_task = task;
            }
        }
    }

    return selected_task ? selected_task.get() : nullptr;
}

std::string EDFScheduler::getName() const{
    return "EDFScheduler";
}
