#include <gtest/gtest.h>
#include <Kernel.hpp>
#include <Sensor.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <RMScheduler.hpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>

CABuffer<int, 20> sensor_data_buffer;

class PreemptionTrackingTest : public ::testing::Test {
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

TEST_F(PreemptionTrackingTest, CountContextSwitches) {
    // id, arrival, computation, deadline, criticality, value
    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 4, 10, TaskCriticality::HARD, 10); // Low rel deadline
    auto t2 = std::make_unique<TaskControlBlock>(1, 1, 2, 9, TaskCriticality::FIRM, 8); // Will preempt t1

    t1->bindBehaviour(producer);
    t2->bindBehaviour(producer);

    kernel->addTask(std::move(t1));
    kernel->addTask(std::move(t2));

    kernel->runPreemptive(10);
    EXPECT_EQ(kernel->getContextSwitchCount(), 1);
}
