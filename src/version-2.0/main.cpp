#include <Kernel.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <RMScheduler.hpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>

// Shared buffer for sensor data
CABuffer<int, 10> sensor_data_buffer;

int main() {
    // Initialize main variables
    ConsoleLogger logger("Main");
    logger.log("RTOS version 2.0 started.");

    // auto sensor = Sensor(1);
    auto scheduler = std::make_unique<RMScheduler>();
    auto kernel = Kernel(std::move(scheduler));
    // auto producer = ProducerTask(sensor);
    // auto consumer = ConsumerTask();
    
    // Task: id, arrival time, computation time, period, criticality, value
    // Task 0: arrives at 0, computation 6, period 20 (LOW priority)
    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 6, 20, TaskCriticality::HARD, 10); // Task ID 0
    // Task 1: arrives at 2, computation 3, period 10 (HIGH priority)
    auto t2 = std::make_unique<TaskControlBlock>(1, 2, 3, 10, TaskCriticality::FIRM, 8);  // Task ID 1
    // Task 2: arrives at 5, computation 2, period 15 (MEDIUM priority)
    auto t3 = std::make_unique<TaskControlBlock>(2, 5, 2, 15, TaskCriticality::SOFT, 6);  // Task ID 2

    // Bind behaviours to tasks
    // t1->bindBehaviour(&producer);
    // t2->bindBehaviour(&consumer);

    // Add tasks to the kernel
    kernel.addTask(std::move(t1));
    kernel.addTask(std::move(t2));
    kernel.addTask(std::move(t3));

    // Run the kernel
    kernel.runPreemptive(15);

    logger.log("RTOS version 2.0 finished executing tasks.\n");
    return 0;
}