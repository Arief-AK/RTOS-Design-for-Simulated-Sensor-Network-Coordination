#ifndef CONSUMER_TASK_HPP
#define CONSUMER_TASK_HPP

#include <iostream>
#include <thread>
#include <chrono>

#include <TaskControlBlock.hpp>
#include <TaskBehaviour.hpp>

#include <CABuffer.hpp>
#include <CABuffer.tpp>

#include <config.hpp>
#include <ConsoleLogger.hpp>
#include <Sensor.hpp>

// Simulate shared resource
extern CABuffer<int, BUFFER_SIZE> sensor_data_buffer;

class ConsumerTask : public TaskBehaviour {
public:
    ConsumerTask(std::unique_ptr<Logger> logger = std::make_unique<ConsoleLogger>("ConsumerTask", true));
    void run(TaskControlBlock* task, uint8_t current_time) override;

private:
    std::unique_ptr<Logger> m_logger;
};

#endif // CONSUMER_TASK_HPP