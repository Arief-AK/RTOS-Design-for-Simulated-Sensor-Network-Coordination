#include <ExperimentRunner.hpp>

ExperimentRunner::ExperimentRunner(uint8_t simulation_time) : m_simulation_time(simulation_time) {}

ExperimentRunner::~ExperimentRunner() = default;

void ExperimentRunner::addScheduler(std::unique_ptr<Scheduler> scheduler){
    m_schedulers.push_back(std::move(scheduler));
}

void ExperimentRunner::setTaskSet(const std::vector<std::unique_ptr<TaskControlBlock>> &task_set){
    m_task_set.clear();
    for (auto &task : task_set){
        m_task_set.push_back(task->clone());
    }
}

void ExperimentRunner::runAll(){
    for (auto &scheduler : m_schedulers){
        _runExperiment(scheduler.get(), scheduler->getName());
    }
}

std::vector<std::unique_ptr<TaskControlBlock>> ExperimentRunner::_cloneTaskSet() const{
    std::vector<std::unique_ptr<TaskControlBlock>> clones;
    for (const auto &task : m_task_set){
        clones.push_back(task->clone());
    }
    return clones;
}

void ExperimentRunner::_runExperiment(Scheduler *scheduler, const std::string &scheduler_name){
    // Initialise kernel and tasks
    auto kernel = std::make_unique<Kernel>(scheduler->clone());
    auto tasks = _cloneTaskSet();

    // Add tasks to the kernel
    for (auto &task : tasks){
        kernel->addTask(std::move(task));
    }

    // Run simulation
    kernel->run(m_simulation_time);
}
