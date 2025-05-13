#include <gtest/gtest.h>
#include <RoundRobinScheduler.hpp>
#include <PriorityScheduler.hpp>
#include <EDFScheduler.hpp>
#include <SimulationEngine.hpp>

class SchedulerResourceBlockingTest : public ::testing::TestWithParam<std::function<std::unique_ptr<Scheduler>()>> {};

// Instantiate the test with different schedulers
INSTANTIATE_TEST_SUITE_P(SchedulerTypes, SchedulerResourceBlockingTest, ::testing::Values(
    []() { return std::make_unique<RoundRobinScheduler>(2); },
    []() { return std::make_unique<PriorityScheduler>(); },
    []() { return std::make_unique<EDFScheduler>(); }
));

TEST_P(SchedulerResourceBlockingTest, ResourceBlocking) {
    std::cout << "********* Resource Blocking Scenario *********\n" << std::endl;

    // Initialise TaskController
    TaskController task_controller;

    // Add sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 4, 10, 2, 1)); // Requires resource); // Arrives late
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 1, 2, 10, 1, 1)); // Also requires same resource

    // Create simulation engine with all scheduler
    SimulationEngine engine(
        "Test_SchedulerResourceBlocking",
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("Test_SchedulerResourceBlocking"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Assertions for resource blocking
    EXPECT_EQ(completed_tasks.size(), 2); // Both tasks should complete
    EXPECT_EQ(engine.getCurrentTime(), 10); // All tasks should complete by max_ticks
    EXPECT_EQ(engine.getContextSwitchCount(), 1); // Two context switches for two tasks
}