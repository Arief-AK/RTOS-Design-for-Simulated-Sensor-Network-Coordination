#ifndef SIMULATION_ENGINE_HPP
#define SIMULATION_ENGINE_HPP

#include <Scheduler.hpp>
#include <TaskController.hpp>

class SimulationEngine{
public:
    SimulationEngine(std::unique_ptr<TaskController> task_controller, std::unique_ptr<Scheduler> scheduler, int max_ticks = 100);
    ~SimulationEngine();

    void run();
    
    int getCurrentTime() const;
    void printStatistics() const;

    std::vector<std::shared_ptr<TaskControlBlock>> getCompletedTasks() const;

private:
    std::unique_ptr<TaskController> m_task_controller;
    std::unique_ptr<Scheduler> m_scheduler;
    std::vector<std::shared_ptr<TaskControlBlock>> m_task_list;

    int m_current_time;
    int m_max_ticks;
};

#endif // SIMULATION_ENGINE_HPP