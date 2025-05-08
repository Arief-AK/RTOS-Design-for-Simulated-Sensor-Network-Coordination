#include <gtest/gtest.h>
#include <RoundRobinScheduler.hpp>
#include <PriorityScheduler.hpp>
#include <EDFScheduler.hpp>
#include <SimulationEngine.hpp>

class SchedulerCPUIdleTest : public ::testing::TestWithParam<std::function<std::unique_ptr<Scheduler>()>> {};

// Instantiate the test with different schedulers
INSTANTIATE_TEST_SUITE_P(SchedulerTypes, SchedulerCPUIdleTest, ::testing::Values(
    []() { return std::make_unique<RoundRobinScheduler>(2); },
    []() { return std::make_unique<PriorityScheduler>(); },
    []() { return std::make_unique<EDFScheduler>(); }
));

TEST_P(SchedulerCPUIdleTest, CPUIdleTime) {
    std::cout << "********* CPU Idle Time Scenario *********\n" << std::endl;

    // Initialise TaskController
    TaskController task_controller;

    // Add sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 3, 2, 10, 1, 0)); // Arrives late

    // Create simulation engine with all scheduler
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("Test_SchedulerCPUIdle"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();

    // Assertions for CPU idle time
    EXPECT_GT(engine.getCurrentTime(), 0);
    EXPECT_GE(engine.getCPUIdleTime(), 1); // Ensure some idle time
}

TEST_P(SchedulerCPUIdleTest, NoTasks) {
    std::cout << "********* No Tasks Scenario *********\n" << std::endl;

    // Initialise TaskController with no tasks
    TaskController task_controller;

    // Create simulation engine
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("Test_NoTasks"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();

    // Assertions for CPU idle time
    EXPECT_EQ(engine.getCPUIdleTime(), 10); // All time should be idle
}

TEST_P(SchedulerCPUIdleTest, OverlappingTasks) {
    std::cout << "********* Overlapping Tasks Scenario *********\n" << std::endl;

    // Initialise TaskController
    TaskController task_controller;

    // Add overlapping tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 3, 2, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::PERIODIC, 3, 2, 10, 1, 0));

    // Create simulation engine
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("Test_OverlappingTasks"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();

    // Assertions for CPU idle time
    EXPECT_EQ(engine.getCPUIdleTime(), 0); // No idle time expected
}

TEST_P(SchedulerCPUIdleTest, HighPriorityPreemption) {
    std::cout << "********* High Priority Preemption Scenario *********\n" << std::endl;

    // Initialise TaskController
    TaskController task_controller;

    // Add tasks with different priorities
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 3, 2, 10, 1, 0)); // Low priority
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 1, 1, 5, 2, 0));  // High priority

    // Create simulation engine
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("Test_HighPriorityPreemption"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();

    // Assertions for CPU idle time
    EXPECT_GE(engine.getCPUIdleTime(), 0); // Ensure no unexpected idle time
}