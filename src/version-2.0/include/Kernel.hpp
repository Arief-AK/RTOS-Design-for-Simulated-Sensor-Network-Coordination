#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <vector>
#include <memory>
#include <Logger.hpp>
#include <Scheduler.hpp>
#include <Dispatcher.hpp>

class Kernel{
public:
    Kernel(
        std::unique_ptr<Scheduler> scheduler,
        std::unique_ptr<Logger> logger = std::make_unique<ConsoleLogger>("Kernel", true)
    );
    ~Kernel();

    void addTask(std::unique_ptr<TaskControlBlock> task);
    void run(u_int8_t simulation_time);

private:
    std::vector<std::unique_ptr<TaskControlBlock>> m_taskList;
    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<Dispatcher> m_dispatcher;
    std::unique_ptr<Logger> m_logger;
};

#endif // KERNEL_HPP