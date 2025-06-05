#ifndef PRODUCER_TASK_HPP
#define PRODUCER_TASK_HPP

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
extern CABuffer<int, 20> sensor_data_buffer;

class ProducerTask : public TaskBehaviour{
public:
    ProducerTask(Sensor& sensor, std::unique_ptr<Logger> logger = std::make_unique<ConsoleLogger>("ProducerTask", true));
    void run(TaskControlBlock* task, uint8_t current_time) override;

private:
    Sensor& m_sensor;
    std::unique_ptr<Logger> m_logger;
};

#endif // PRODUCER_TASK_HPP