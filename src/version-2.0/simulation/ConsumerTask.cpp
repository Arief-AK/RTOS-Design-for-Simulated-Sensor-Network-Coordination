#include <ConsumerTask.hpp>

ConsumerTask::ConsumerTask(std::unique_ptr<Logger> logger)
:   m_logger(logger ? std::move(logger) : std::make_unique<ConsoleLogger>("ConsumerTask", true)) {}

void ConsumerTask::run(TaskControlBlock* task, uint8_t current_time) {
    if(task->getRemainingTime() == 0) return;
    
    task->setStatus(TaskStatus::RUNNING);

    if (ENABLE_REAL_TIME_SIMULATION) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(TIME_UNIT_MS));
        std::this_thread::sleep_for(std::chrono::milliseconds(task->getComputationTime() * TIME_UNIT_MS));
    }

    auto data = sensor_data_buffer.pop();
    if (data) {
        m_logger->log("Time : " + std::to_string(current_time) + 
                      " - Consumer Task " + std::to_string(task->getTaskId()) + 
                      " consumed data: " + std::to_string(*data));
    } else {
        m_logger->log("Time : " + std::to_string(current_time) + 
                      " - Consumer Task " + std::to_string(task->getTaskId()) + 
                      " found buffer empty, no data to consume.");
    }

    task->decrementRemainingTime();

    if(task->getRemainingTime() == 0){
        task->setFinishTime(current_time + 1);
        task->updateMetrics(task->getFinishTime());
        task->setStatus(TaskStatus::COMPLETED);
    } else {
        task->setStatus(TaskStatus::READY);
    }
}