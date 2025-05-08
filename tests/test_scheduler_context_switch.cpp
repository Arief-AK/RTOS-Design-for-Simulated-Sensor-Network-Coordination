#include <gtest/gtest.h>
#include <RoundRobinScheduler.hpp>
#include <PriorityScheduler.hpp>
#include <EDFScheduler.hpp>
#include <SimulationEngine.hpp>

class SchedulerContextSwitchTest : public ::testing::TestWithParam<std::function<std::unique_ptr<Scheduler>()>> {};

// Instantiate the test with different schedulers
INSTANTIATE_TEST_SUITE_P(SchedulerTypes, SchedulerContextSwitchTest,::testing::Values(
    []() {return std::make_unique<RoundRobinScheduler>(2); },
    []() {return std::make_unique<PriorityScheduler>(); },
    []() {return std::make_unique<EDFScheduler>(); }
));

TEST_P(SchedulerContextSwitchTest, ContextSwitchCount){
    std::cout << "********* Context Switch Count Scenario *********\n" << std::endl;
    
    // Initialise TaskController
    TaskController task_controller;

    // Add sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 1, 2, 10, 2, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 2, 1, 10, 3, 0));

    // Create simulation engine with all scheduler
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("Test_SchedulerContextSwitch"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();
    
    // Assertions for context switch count
    EXPECT_EQ(engine.getContextSwitchCount(), engine.getCompletedTasks().size() - 1);
}