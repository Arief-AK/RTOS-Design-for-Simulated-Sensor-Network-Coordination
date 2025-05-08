#include <Controller.hpp>

Controller::Controller(const std::string& name, int max_ticks) 
    : m_controller_name(name), m_max_ticks(max_ticks)
{
    m_task_controller = std::make_unique<TaskController>();
    m_logger = std::make_shared<ConsoleLogger>(name);
}

Controller::~Controller(){}

void Controller::addTask(
    int id, TaskType type, int period,
    int execution, int deadline, int priority, int arrival){
    auto task = std::make_shared<TaskControlBlock>(
        id, type, period, execution, deadline, priority, arrival);

    m_task_controller->addTask(task);
}

void Controller::displayTasks(){
    m_task_controller->displayTasks();
}

void Controller::displayCompletedTasks(){
    m_engine->getCompletedTasks();
}

void Controller::createScheduler(SchedulerType type){
    switch (type)
    {
    case SchedulerType::ROUND_ROBIN:
        m_scheduler = std::make_unique<RoundRobinScheduler>(2);
        break;

    case SchedulerType::PRIORITY:
        m_scheduler = std::make_unique<PriorityScheduler>();
        break;

    case SchedulerType::EDF:
        m_scheduler = std::make_unique<EDFScheduler>();
        break;
    
    default:
        throw std::invalid_argument("Invalid scheduler type");
        break;
    }
}

SchedulerType Controller::parseSchedulerType(const std::string& input){
    if (input == "RR" || input == "Round Robin") {
        return SchedulerType::ROUND_ROBIN;
    } else if (input == "PR" || input == "Priority") {
        return SchedulerType::PRIORITY;
    } else if (input == "EDF" || input == "Earliest Deadline First") {
        return SchedulerType::EDF;
    } else {
        throw std::invalid_argument("Invalid scheduler type:" + input);
    }
}

void Controller::runSimulation(){
    _createEngine();
    m_engine->run();
}

void Controller::_createEngine(){
    // Create the simulation engine
    m_engine = std::make_unique<SimulationEngine>(
        std::move(m_task_controller),
        std::move(m_scheduler),
        m_logger,
        m_max_ticks);
}
