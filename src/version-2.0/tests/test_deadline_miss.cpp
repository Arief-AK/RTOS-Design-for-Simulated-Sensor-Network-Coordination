#include <gtest/gtest.h>
#include <Kernel.hpp>
#include <Sensor.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <RMScheduler.hpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>

CABuffer<int, 20> sensor_data_buffer;

class DeadlineMissTest : public ::testing::Test{
protected:
    std::unique_ptr<Kernel> kernel;
    Sensor sensor {1};
    std::shared_ptr<ProducerTask> producer;

    void SetUp() override{
        auto scheduler = std::make_unique<RMScheduler>();
        kernel = std::make_unique<Kernel>(std::move(scheduler), nullptr);

        sensor = Sensor(1);
        producer = std::make_shared<ProducerTask>(sensor, nullptr);
    }
};

TEST_F(DeadlineMissTest, TaskMissesDeadline) {
    // Task arrives late and has insufficient time to complete before its deadline
    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 5, 4, TaskCriticality::HARD, 10);
    t1->bindBehaviour(producer);

    kernel->addTask(std::move(t1));
    kernel->runPreemptive(6);

    const auto& task = kernel->getTaskList().at(0);
    EXPECT_TRUE(task->isCompleted());
    EXPECT_GT(task->getFinishTime(), task->getAbsDeadline());
    EXPECT_GT(task->getTardiness(), 0);
    EXPECT_GT(task->getLateness(), 0);
}