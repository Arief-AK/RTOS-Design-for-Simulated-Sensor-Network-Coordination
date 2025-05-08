#ifndef SIMULATION_ENGINE_HPP
#define SIMULATION_ENGINE_HPP

#include <Resource.hpp>
#include <Scheduler.hpp>
#include <TaskController.hpp>
#include <MetricsCollector.hpp>

class SimulationEngine{
public:
    SimulationEngine(
        std::unique_ptr<TaskController> task_controller,
        std::unique_ptr<Scheduler> scheduler,
        std::shared_ptr<Logger> logger,
        int max_ticks = 100);
    ~SimulationEngine();

    void run();
    
    int getCurrentTime() const;
    int getContextSwitchCount() const;
    int getCPUIdleTime() const;

    std::vector<std::shared_ptr<TaskControlBlock>> getCompletedTasks() const;

private:
    std::unique_ptr<TaskController> m_task_controller;
    std::unique_ptr<Scheduler> m_scheduler;
    std::shared_ptr<Logger> m_logger;
    std::unique_ptr<MetricsCollector> m_metrics_collector;
    std::vector<std::shared_ptr<TaskControlBlock>> m_task_list;

    std::shared_ptr<Resource> m_resource;

    int m_current_time;
    int m_max_ticks;
};

#endif // SIMULATION_ENGINE_HPP