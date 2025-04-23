#include <RoundRobinScheduler.hpp>

RoundRobinScheduler::RoundRobinScheduler(int time_quantum)
    : m_time_quantum(time_quantum), m_last_index(0) {}

TaskControlBlock* RoundRobinScheduler::selectTask(
    std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue){
    // Check if tasks are empty
    if (ready_queue.empty()){
        return nullptr;
    }

    // Round-robin: Select the next READY task starting from the last index
    size_t num_tasks = ready_queue.size();
    for (size_t i = 0; i < num_tasks; i++){
        size_t index = (m_last_index + i) % num_tasks;
        if(ready_queue[index]->status == TaskStatus::READY){
            m_last_index = (index + 1) % num_tasks;                 // Update the last index for the next selection
            ready_queue[index]->status = TaskStatus::RUNNING;       // Set the task to RUNNING
            return ready_queue[index].get();                        // Return the selected task
        }
    }
    
    return nullptr; // No task found
}

std::string RoundRobinScheduler::getName() const{
    return "Round Robin Scheduler";
}