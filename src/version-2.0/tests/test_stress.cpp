#include <gtest/gtest.h>
#include <Kernel.hpp>
#include <Sensor.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <RMScheduler.hpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>

CABuffer<int, 20> sensor_data_buffer;

class StressRTOSSimulationTest : public ::testing::Test {
protected:
    std::unique_ptr<Kernel> kernel;
    Sensor sensor {1};
    std::shared_ptr<ProducerTask> producer;

    void SetUp() override {
        auto scheduler = std::make_unique<RMScheduler>();
        kernel = std::make_unique<Kernel>(std::move(scheduler), nullptr);

        sensor = Sensor(1);
        producer = std::make_shared<ProducerTask>(sensor, nullptr);
    }
};

TEST_F(StressRTOSSimulationTest, HandlesHighTaskVolume) {
    const int task_count = 20;

    for (int i = 0; i < task_count; ++i) {
        // id, arrival, computation, deadline, criticality, value
        auto task = std::make_unique<TaskControlBlock>(
            i, i % 5, 2, (uint8_t)(i % 5 + 6), TaskCriticality::FIRM, 5);
        task->bindBehaviour(producer);
        kernel->addTask(std::move(task));
    }

    kernel->runPreemptive(50);

    for (const auto& task : kernel->getTaskList()) {
        EXPECT_TRUE(task->isCompleted());
    }
}
