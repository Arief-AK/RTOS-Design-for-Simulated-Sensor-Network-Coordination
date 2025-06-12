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

void Controller::RunPriorityScheduler(){
    m_logger.log("******************** START OF PriorityScheduler ********************");

    auto sensor = Sensor(1);
    auto scheduler = std::make_unique<PriorityScheduler>();
    auto kernel = Kernel(std::move(scheduler));
    auto producer = std::make_shared<ProducerTask>(sensor);
    auto consumer = std::make_shared<ConsumerTask>();

    // id, arrival, computation, deadline, criticality, value
    auto p0 = std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 10); // Producer
    auto p1 = std::make_unique<TaskControlBlock>(2, 3, 2, 12, TaskCriticality::HARD, 10);
    auto c0 = std::make_unique<TaskControlBlock>(1, 2, 1, 9, TaskCriticality::FIRM, 8);  // Consumer
    auto c1 = std::make_unique<TaskControlBlock>(5, 2, 3, 13, TaskCriticality::FIRM, 8);  // Consumer

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

    // Run the kernel
    kernel.runPreemptive(10);

    m_logger.log("******************** END OF PriorityScheduler ********************\n");
}
