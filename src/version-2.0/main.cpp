#include <Kernel.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <RMScheduler.hpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>

// Shared buffer for sensor data
CABuffer<int, 10> sensor_data_buffer;

int main() {
    // Initialize the logger
    ConsoleLogger logger("Main");
    logger.log("RTOS version 2.0 started.");

    // Initialise sensor and tasks
    Sensor sensor(1);
    ProducerTask producer(sensor, std::make_unique<ConsoleLogger>("ProducerTask"));
    ConsumerTask consumer(std::make_unique<ConsoleLogger>("ConsumerTask"));
    
    auto scheduler = std::make_unique<RMScheduler>();
    auto kernel = Kernel(std::move(scheduler));

    // kernel.addTask(std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 10)); // producer
    // kernel.addTask(std::make_unique<TaskControlBlock>(1, 1, 3, 15, TaskCriticality::FIRM, 8));  // consumer

    //kernel.run(50);

    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 10);  // Producer
    auto t2 = std::make_unique<TaskControlBlock>(1, 1, 3, 15, TaskCriticality::FIRM, 8);   // Consumer

    TaskControlBlock* t1_ptr = t1.get();
    TaskControlBlock* t2_ptr = t2.get();

    for (uint8_t tick = 0; tick < 10; ++tick){
        logger.log("Tick: " + std::to_string(tick));
        if(tick == t1_ptr->getArrivalTime()) producer.run(t1_ptr, tick);
        if(tick == t2_ptr->getArrivalTime()) consumer.run(t2_ptr, tick);
    }
    
    logger.log("RTOS version 2.0 finished executing tasks.\n");
    return 0;
}