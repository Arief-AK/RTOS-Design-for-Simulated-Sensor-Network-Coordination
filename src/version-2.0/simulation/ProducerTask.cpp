#include <ProducerTask.hpp>

ProducerTask::ProducerTask(Sensor &sensor, std::unique_ptr<Logger> logger)
:   m_sensor(sensor),
    m_logger(logger ? std::move(logger) : std::make_unique<ConsoleLogger>("ProducerTask", true)) {}

void ProducerTask::run(TaskControlBlock *task, uint8_t current_time) {
    if(task->getRemainingTime() == 0) return;
  
    task->setStatus(TaskStatus::RUNNING);

    if(ENABLE_REAL_TIME_SIMULATION){
        // std::this_thread::sleep_for(std::chrono::milliseconds(TIME_UNIT_MS));
        std::this_thread::sleep_for(std::chrono::milliseconds(task->getComputationTime() * TIME_UNIT_MS));
    }

    auto value = m_sensor.read_value();
    auto success = sensor_data_buffer.push(value);
    m_logger->log("Time : " + std::to_string(current_time) + 
                    " - Producer Task " + std::to_string(task->getTaskId()) + 
                    " sensor value: " + std::to_string(value) + 
                    (success ? " and pushed to buffer." : " but buffer is full, could not push.")
                );

    task->decrementRemainingTime();

    if(task->getRemainingTime() == 0){
        task->setFinishTime(current_time + 1);
        task->updateMetrics(task->getFinishTime());
        task->setStatus(TaskStatus::COMPLETED);
    } else {
        task->setStatus(TaskStatus::READY);
    }
}