#include "Dispatcher.hpp"

Dispatcher::Dispatcher() : m_logger(std::make_unique<ConsoleLogger>("Dispatcher")) {}

Dispatcher::~Dispatcher(){}

void Dispatcher::dispatch(TaskControlBlock* task, u_int8_t current_time){
    if (!task){
        throw std::invalid_argument("Task is null");
    }

    // Set task status to RUNNING
    task->setStatus(TaskStatus::RUNNING);
    m_logger->log("Executing task with ID: " + std::to_string(task->getTaskId()) + 
                  " at time: " + std::to_string(current_time));

    if(ENABLE_REAL_TIME_SIMULATION){
        std::chrono::milliseconds delay(task->getComputationTime() * TIME_UNIT_MS);
        std::this_thread::sleep_for(delay);
    }

    // Update task metrics after execution
    task->updateMetrics(current_time);
    task->setStatus(TaskStatus::COMPLETED);
    m_logger->log("Completed task with ID: " + std::to_string(task->getTaskId()) + 
                  " at time: " + std::to_string(current_time + task->getComputationTime()));
}
