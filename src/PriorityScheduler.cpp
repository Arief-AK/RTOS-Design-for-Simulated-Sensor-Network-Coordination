#include <PriorityScheduler.hpp>

PriorityScheduler::PriorityScheduler() : m_last_index(0) {}

PriorityScheduler::~PriorityScheduler(){}

TaskControlBlock *PriorityScheduler::selectTask(
    std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue) {
    std::shared_ptr<TaskControlBlock> selected_task = nullptr;
    std::shared_ptr<TaskControlBlock> running_task = nullptr;

    // Find currently running task if any
    for (const auto& task : ready_queue) {
        if (task->status == TaskStatus::RUNNING) {
            running_task = task;
            // Keep running only if it has time remaining
            if (task->remaining_time > 0) {
                selected_task = task;
            }
            break;
        }
    }

    // If no task is running or current task is done, select highest priority ready task
    if (!selected_task) {
        int highest_prio = std::numeric_limits<int>::max(); // Higher numbers mean lower priority
        int earliest_arrival = std::numeric_limits<int>::max();
        
        // Find highest priority task (lowest number) that's ready
        for (const auto& task : ready_queue) {
            if (task->status == TaskStatus::READY && 
                task->remaining_time > 0 &&
                task->priority < highest_prio) {
                highest_prio = task->priority;
            }
        }

        // Among tasks with highest priority, select one with earliest arrival
        for (const auto& task : ready_queue) {
            if (task->status == TaskStatus::READY && 
                task->remaining_time > 0 &&
                task->priority == highest_prio &&
                task->arrival_time < earliest_arrival) {
                earliest_arrival = task->arrival_time;
                selected_task = task;
            }
        }
    }
    // If a task is running, check for preemption by higher priority task
    else if (running_task && running_task->remaining_time > 0) {
        int highest_prio = running_task->priority;
        int earliest_arrival = std::numeric_limits<int>::max();
        std::shared_ptr<TaskControlBlock> preempting_task = nullptr;

        // Check for any ready tasks with higher priority (lower number)
        for (const auto& task : ready_queue) {
            if (task->status == TaskStatus::READY && 
                task->remaining_time > 0 &&
                task->priority < highest_prio) { // Lower number = higher priority
                highest_prio = task->priority;
            }
        }

        // If found a higher priority task, select earliest arrival among them
        if (highest_prio < running_task->priority) {
            for (const auto& task : ready_queue) {
                if (task->status == TaskStatus::READY && 
                    task->remaining_time > 0 &&
                    task->priority == highest_prio &&
                    task->arrival_time < earliest_arrival) {
                    earliest_arrival = task->arrival_time;
                    preempting_task = task;
                }
            }

            if (preempting_task) {
                running_task->status = TaskStatus::READY;
                selected_task = preempting_task;
            }
        }
    }

    // Mark selected task as running
    if (selected_task) {
        selected_task->status = TaskStatus::RUNNING;
    }

    return selected_task ? selected_task.get() : nullptr;
}

std::string PriorityScheduler::getName() const{
    return "PriorityScheduler";
}
