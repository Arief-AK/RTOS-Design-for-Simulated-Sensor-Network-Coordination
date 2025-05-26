#ifndef PRODUCER_TASK_HPP
#define PRODUCER_TASK_HPP

#include <iostream>
#include <thread>
#include <chrono>

#include <TaskControlBlock.hpp>
#include <CABuffer.hpp>
#include <config.hpp>
#include <ConsoleLogger.hpp>
#include <Sensor.hpp>

// Simulate shared resource
extern CABuffer<int, 10> shared_buffer;

class ProducerTask{
public:
    ProducerTask(Sensor& sensor, std::unique_ptr<Logger> logger)
    :   m_sensor(sensor),
        m_logger(logger ? std::move(logger) : std::make_unique<ConsoleLogger>("ProducerTask")
    ) {};

    void run(TaskControlBlock* task, uint8_t current_time){
        task->setStatus(TaskStatus::RUNNING);

        if(ENABLE_REAL_TIME_SIMULATION){
            std::this_thread::sleep_for(std::chrono::milliseconds(task->getComputationTime() * TIME_UNIT_MS));
        }

        auto value = m_sensor.read_value();
        auto success = shared_buffer.push(value);
        m_logger->log("Time : " + std::to_string(current_time) + 
                      " - Producer Task " + std::to_string(task->getTaskId()) + 
                      " sensor value: " + std::to_string(value) + 
                      (success ? " and pushed to buffer." : " but buffer is full, could not push.")
                    );

        task->updateMetrics(current_time);
        task->setStatus(TaskStatus::COMPLETED);
    }

private:
    Sensor& m_sensor;
    std::unique_ptr<Logger> m_logger;
};

#endif // PRODUCER_TASK_HPP