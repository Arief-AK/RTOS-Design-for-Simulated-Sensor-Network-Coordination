#include <Kernel.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <RMScheduler.hpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>
#include <memory>

// Shared buffer for sensor data
CABuffer<int, 10> sensor_data_buffer;

int main() {
    // Initialize main variables
    ConsoleLogger logger("Main");
    logger.log("RTOS version 2.0 started.");

    auto sensor = Sensor(1);
    auto scheduler = std::make_unique<RMScheduler>();
    auto kernel = Kernel(std::move(scheduler));
    auto producer = std::make_shared<ProducerTask>(sensor);
    auto consumer = std::make_shared<ConsumerTask>();

    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 3, 10, TaskCriticality::HARD, 10); // Producer
    auto t2 = std::make_unique<TaskControlBlock>(1, 1, 3, 15, TaskCriticality::FIRM, 8);  // Consumer

    // Bind behaviours to tasks
    t1->bindBehaviour(producer);
    t2->bindBehaviour(consumer);

    // Add tasks to the kernel
    kernel.addTask(std::move(t1));
    kernel.addTask(std::move(t2));

    // Run the kernel
    kernel.runPreemptive(10);

    logger.log("RTOS version 2.0 finished executing tasks.\n");
    return 0;
}