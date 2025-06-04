#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <queue>
#include <vector>
#include <memory>
#include <cstdint>

#include <Logger.hpp>
#include <Scheduler.hpp>
#include <Dispatcher.hpp>
#include <JSONLogger.hpp>
#include <MetricsCollector.hpp>

class Kernel{
public:
    Kernel(
        std::unique_ptr<Scheduler> scheduler,
        std::unique_ptr<Logger> logger = std::make_unique<ConsoleLogger>("Kernel", true)
    );
    ~Kernel();

    // Task management
    void addTask(std::unique_ptr<TaskControlBlock> task);
    
    // Kernel execution methods
    void run(uint8_t simulation_time);
    void runPreemptive(uint8_t simulation_time);

    // Accessor methods
    uint32_t getContextSwitchCount() const;
    const std::vector<std::unique_ptr<TaskControlBlock>>& getTaskList() const;

private:
    std::vector<std::unique_ptr<TaskControlBlock>> m_taskList;
    std::queue<TaskControlBlock*> m_ready_queue;

    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<Dispatcher> m_dispatcher;

    std::unique_ptr<Logger> m_logger;
    std::unique_ptr<MetricsCollector> m_metricsCollector;
    std::unique_ptr<JSONLogger> m_json_logger; // JSON logger for structured logging

    void _update_ready_queue(uint8_t tick);

    TaskControlBlock* m_current_task = nullptr;
    uint32_t m_context_switch_count = 0;
};

#endif // KERNEL_HPP