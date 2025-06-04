#include <gtest/gtest.h>
#include <Kernel.hpp>
#include <Sensor.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <RMScheduler.hpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>

CABuffer<int, 10> sensor_data_buffer;

class PreemptiveKernelTest : public ::testing::Test{
protected:
    std::unique_ptr<Kernel> kernel;
    Sensor sensor {1}; // Sensor with ID 1
    std::shared_ptr<ProducerTask> producer;
    std::shared_ptr<ConsumerTask> consumer;

    void SetUp() override {
        auto scheduler = std::make_unique<RMScheduler>();
        kernel = std::make_unique<Kernel>(std::move(scheduler), nullptr);

        sensor = Sensor(1);
        producer = std::make_shared<ProducerTask>(sensor, nullptr);
        consumer = std::make_shared<ConsumerTask>(nullptr);
    }
};

TEST_F(PreemptiveKernelTest, ProducerConsumerExecutionOrder) {
    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 10);
    auto t2 = std::make_unique<TaskControlBlock>(1, 1, 2, 15, TaskCriticality::FIRM, 8);

    t1->bindBehaviour(producer);
    t2->bindBehaviour(consumer);

    kernel->addTask(std::move(t1));
    kernel->addTask(std::move(t2));

    kernel->runPreemptive(10);

    const auto& taskList = kernel->getTaskList();

    ASSERT_EQ(taskList.size(), 2);
    EXPECT_TRUE(taskList[0]->isCompleted());
    EXPECT_TRUE(taskList[1]->isCompleted());
    EXPECT_LE(taskList[0]->getStartTime(), taskList[1]->getStartTime());
}

TEST_F(PreemptiveKernelTest, BufferInteraction) {
    sensor_data_buffer.clear();
    EXPECT_TRUE(sensor_data_buffer.is_empty());

    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 1, 10, TaskCriticality::HARD, 10);
    auto t2 = std::make_unique<TaskControlBlock>(1, 1, 1, 15, TaskCriticality::FIRM, 8);

    t1->bindBehaviour(producer);
    t2->bindBehaviour(consumer);

    kernel->addTask(std::move(t1));
    kernel->addTask(std::move(t2));

    kernel->runPreemptive(10);

    // One value produced and consumed
    EXPECT_TRUE(sensor_data_buffer.is_empty());
}

TEST_F(PreemptiveKernelTest, TaskMetricsRecordedCorrectly) {
    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 3, 10, TaskCriticality::HARD, 10);
    t1->bindBehaviour(producer);

    kernel->addTask(std::move(t1));
    kernel->runPreemptive(10);

    const auto& task = kernel->getTaskList().at(0);
    EXPECT_TRUE(task->isCompleted());
    EXPECT_EQ(task->getRemainingTime(), 0);
    EXPECT_EQ(task->getFinishTime(), task->getStartTime() + task->getComputationTime());
}
