#include <Controller.hpp>

Controller::Controller() : m_logger("Controller", true) {}

void Controller::RunRMScheduler(){
    m_logger.log("******************** START OF RMScheduler ********************");

    auto sensor = Sensor(1);
    auto scheduler = std::make_unique<RMScheduler>();
    auto kernel = Kernel(std::move(scheduler));
    auto producer = std::make_shared<ProducerTask>(sensor);
    auto consumer = std::make_shared<ConsumerTask>();

    // // id, arrival, computation, deadline, criticality, value
    auto p0 = std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 10); // Producer
    auto p1 = std::make_unique<TaskControlBlock>(2, 3, 2, 12, TaskCriticality::HARD, 10);
    
    auto c0 = std::make_unique<TaskControlBlock>(1, 2, 1, 9, TaskCriticality::FIRM, 8);  // Consumer
    auto c1 = std::make_unique<TaskControlBlock>(5, 2, 3, 13, TaskCriticality::FIRM, 8);  // Consumer

    // Bind behaviours to tasks
    p0->bindBehaviour(producer);
    p1->bindBehaviour(producer);
    c0->bindBehaviour(consumer);
    c1->bindBehaviour(consumer);

    // Add tasks to the kernel
    kernel.addTask(std::move(p0));
    kernel.addTask(std::move(p1));
    kernel.addTask(std::move(c0));
    kernel.addTask(std::move(c1));

    // Run the kernel
    kernel.runPreemptive(10);

    m_logger.log("******************** END OF RMScheduler ********************\n");
}

void Controller::RunEDFScheduler(){
    m_logger.log("******************** START OF EDFScheduler ********************");

    auto sensor = Sensor(1);
    auto scheduler = std::make_unique<EDFScheduler>();
    auto kernel = Kernel(std::move(scheduler));
    auto producer = std::make_shared<ProducerTask>(sensor);
    auto consumer = std::make_shared<ConsumerTask>();

    // id, arrival, computation, deadline, criticality, value
    auto p0 = std::make_unique<TaskControlBlock>(0, 0, 2, 7, TaskCriticality::HARD, 10); // Producer, short deadline
    auto p1 = std::make_unique<TaskControlBlock>(2, 2, 2, 12, TaskCriticality::HARD, 10); // Producer, longer deadline

    auto c0 = std::make_unique<TaskControlBlock>(1, 1, 1, 5, TaskCriticality::FIRM, 8);  // Consumer, shortest deadline
    auto c1 = std::make_unique<TaskControlBlock>(3, 2, 3, 15, TaskCriticality::FIRM, 8);  // Consumer, longest deadline

    // Bind behaviours to tasks
    p0->bindBehaviour(producer);
    p1->bindBehaviour(producer);
    c0->bindBehaviour(consumer);
    c1->bindBehaviour(consumer);

    // Add tasks to the kernel
    kernel.addTask(std::move(p0));
    kernel.addTask(std::move(p1));
    kernel.addTask(std::move(c0));
    kernel.addTask(std::move(c1));

    // Run the kernel
    kernel.runPreemptive(15);

    m_logger.log("******************** END OF EDFScheduler ********************\n");
}

void Controller::RunPriorityScheduler(){
    m_logger.log("******************** START OF PriorityScheduler ********************");

    auto sensor = Sensor(1);
    auto scheduler = std::make_unique<PriorityScheduler>();
    auto kernel = Kernel(std::move(scheduler));
    auto producer = std::make_shared<ProducerTask>(sensor);
    auto consumer = std::make_shared<ConsumerTask>();

    // id, arrival, computation, deadline, criticality, value
    auto p0 = std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 10);   // Producer
    auto p1 = std::make_unique<TaskControlBlock>(2, 3, 2, 12, TaskCriticality::HARD, 10);   // Producer

    auto c0 = std::make_unique<TaskControlBlock>(1, 2, 1, 9, TaskCriticality::FIRM, 8);     // Consumer
    auto c1 = std::make_unique<TaskControlBlock>(5, 2, 3, 13, TaskCriticality::FIRM, 8);    // Consumer

    // Assign priorities (lower number = higher priority)
    p0->setPriority(1); // Highest priority
    p1->setPriority(2);
    c0->setPriority(3);
    c1->setPriority(4); // Lowest priority

    // Bind behaviours to tasks
    p0->bindBehaviour(producer);
    p1->bindBehaviour(producer);
    c0->bindBehaviour(consumer);
    c1->bindBehaviour(consumer);

    // Add tasks to the kernel
    kernel.addTask(std::move(p0));
    kernel.addTask(std::move(p1));
    kernel.addTask(std::move(c0));
    kernel.addTask(std::move(c1));

    // Run the kernel in preemptive mode
    kernel.runPreemptive(10);

    m_logger.log("******************** END OF PriorityScheduler ********************\n");
}

void Controller::RunComparativeExperiment() {
    m_logger.log("******************** START OF Comparative Experiment ********************");
    
    uint8_t simulation_time = 20; // Example simulation time
    auto runner = ExperimentRunner(simulation_time);

    // Add schedulers to experiment
    runner.addScheduler(std::make_unique<RMScheduler>());
    runner.addScheduler(std::make_unique<EDFScheduler>());
    runner.addScheduler(std::make_unique<PriorityScheduler>());

    // Create and set identical task set
    auto tasks = _createExperimentSampleTaskSet();
    runner.setTaskSet(tasks);

    // Run all experiments
    runner.runAll();

    std::cout << "Experiments completed. Check output JSON files for results." << std::endl;
    
    m_logger.log("******************** END OF Comparative Experiment ********************\n");
}

std::vector<std::unique_ptr<TaskControlBlock>> Controller::_createExperimentSampleTaskSet() const{
    // Create producer and consumer tasks
    auto sensor = Sensor(1);
    auto producer = std::make_shared<ProducerTask>(sensor);
    auto consumer = std::make_shared<ConsumerTask>();

    // id, arrival, computation, deadline, criticality, value
    auto p0 = std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 10);   // Producer
    auto p1 = std::make_unique<TaskControlBlock>(2, 3, 2, 12, TaskCriticality::HARD, 10);   // Producer

    auto c0 = std::make_unique<TaskControlBlock>(1, 2, 1, 9, TaskCriticality::FIRM, 8);     // Consumer
    auto c1 = std::make_unique<TaskControlBlock>(5, 2, 3, 13, TaskCriticality::FIRM, 8);    // Consumer

    // Assign priorities (lower number = higher priority)
    p0->setPriority(1); // Highest priority
    p1->setPriority(2);
    c0->setPriority(3);
    c1->setPriority(4); // Lowest priority

    // Bind behaviours to tasks
    p0->bindBehaviour(producer);
    p1->bindBehaviour(producer);
    c0->bindBehaviour(consumer);
    c1->bindBehaviour(consumer);
    
    // Initialise tasks vector
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;

    // id, arrival, computation, deadline, criticality, value
    tasks.push_back(std::move(p0));
    tasks.push_back(std::move(p1));
    tasks.push_back(std::move(c0));
    tasks.push_back(std::move(c1));

    return tasks;
}
