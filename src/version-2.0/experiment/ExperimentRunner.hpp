#ifndef EXPERIMENT_RUNNER_HPP
#define EXPERIMENT_RUNNER_HPP

#include <Kernel.hpp>
#include <TaskControlBlock.hpp>

class ExperimentRunner{
public:
    ExperimentRunner(uint8_t simulation_time);
    ~ExperimentRunner();

    void addScheduler(std::unique_ptr<Scheduler> scheduler);
    void setTaskSet(const std::vector<std::unique_ptr<TaskControlBlock>>& task_set);
    void runAll();

private:
    uint8_t m_simulation_time;

    std::vector<std::unique_ptr<Scheduler>> m_schedulers;
    std::vector<std::unique_ptr<TaskControlBlock>> m_task_set;

    std::vector<std::unique_ptr<TaskControlBlock>> _cloneTaskSet() const;
    void _runExperiment(Scheduler* scheduler, const std::string& scheduler_name);
};

#endif // EXPERIMENT_RUNNER_HPP
