#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <iostream>
#include <TaskController.hpp>
#include <PriorityScheduler.hpp>
#include <RoundRobinScheduler.hpp>
#include <EDFScheduler.hpp>
#include <SimulationEngine.hpp>

class Controller
{
public:
    Controller(const std::string& name, int max_ticks);
    ~Controller();

    void addTask(int id, TaskType type, int period, int execution, int deadline, int priority, int arrival);
    void displayTasks();
    void displayCompletedTasks();

    void createScheduler(SchedulerType type);
    SchedulerType parseSchedulerType(const std::string& input);

    void runSimulation();

private:
    std::string m_controller_name;
    int m_max_ticks;

    std::unique_ptr<TaskController> m_task_controller;
    std::shared_ptr<Logger> m_logger;
    
    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<SimulationEngine> m_engine;

    void _createEngine();
};

#endif // CONTROLLER_HPP