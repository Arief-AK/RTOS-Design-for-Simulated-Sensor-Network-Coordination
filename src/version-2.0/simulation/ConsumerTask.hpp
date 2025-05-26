#ifndef CONSUMER_TASK_HPP
#define CONSUMER_TASK_HPP

#include <iostream>
#include <thread>
#include <chrono>

#include <TaskControlBlock.hpp>
#include <CABuffer.hpp>
#include <config.hpp>
#include <ConsoleLogger.hpp>
#include <Sensor.hpp>

// Simulate shared resource
extern CABuffer<int, 10> sensor_data_buffer;

class ConsumerTask{
public:
    // TBA

private:
    // TBA
};

#endif // CONSUMER_TASK_HPP