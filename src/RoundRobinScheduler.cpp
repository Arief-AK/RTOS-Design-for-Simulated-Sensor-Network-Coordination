#include <RoundRobinScheduler.hpp>

RoundRobinScheduler::RoundRobinScheduler(int time_quantum)
    : m_time_quantum(time_quantum), m_last_index(0), m_current_quantum(0), m_current_task(nullptr) {}

TaskControlBlock* RoundRobinScheduler::selectTask(
    std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue) {
    // Check if tasks are empty
    if (ready_queue.empty()) {
        m_current_task = nullptr;
        m_current_quantum = 0;
        return nullptr;
    }

    // If current task is running and not completed, let it finish
    if (m_current_task && m_current_task->status == TaskStatus::RUNNING && 
        m_current_task->remaining_time > 0) {
        m_current_quantum++;
        return m_current_task;
    }

    // Find earliest arriving ready task
    m_current_quantum = 0;
    m_current_task = nullptr;
    
    TaskControlBlock* selected_task = nullptr;
    int earliest_arrival = std::numeric_limits<int>::max();
    
    for (const auto& task : ready_queue) {
        if (task->status == TaskStatus::READY && task->arrival_time < earliest_arrival) {
            earliest_arrival = task->arrival_time;
            selected_task = task.get();
        }
    }

    if (selected_task) {
        selected_task->status = TaskStatus::RUNNING;
        m_current_task = selected_task;
        m_current_quantum = 1;
    }

    return selected_task;
}

std::string RoundRobinScheduler::getName() const {
    return "Round Robin Scheduler";
}