#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <thread>
#include <chrono>
#include <config.hpp>
#include <ConsoleLogger.hpp>
#include <TaskControlBlock.hpp>

class Dispatcher{
public:
    Dispatcher();
    ~Dispatcher();

    void dispatch(TaskControlBlock* task, u_int8_t current_time);

private:
    std::unique_ptr<ConsoleLogger> m_logger;
};

#endif // DISPATCHER_HPP

