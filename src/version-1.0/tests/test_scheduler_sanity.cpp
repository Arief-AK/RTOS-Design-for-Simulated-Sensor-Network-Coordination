#include <gtest/gtest.h>
#include <RoundRobinScheduler.hpp>
#include <PriorityScheduler.hpp>
#include <EDFScheduler.hpp>
#include <SimulationEngine.hpp>

class SchedulerSanityTest : public ::testing::TestWithParam<std::function<std::unique_ptr<Scheduler>()>> {};

// Instantiate the test with different schedulers
INSTANTIATE_TEST_SUITE_P(SchedulerTypes, SchedulerSanityTest,::testing::Values(
    []() {return std::make_unique<RoundRobinScheduler>(2); },
    []() {return std::make_unique<PriorityScheduler>(); },
    []() {return std::make_unique<EDFScheduler>(); }
));

TEST_P(SchedulerSanityTest, CompleteAllTasks){
    std::cout << "********* Complete All Tasks Scenario *********\n" << std::endl;
    
    // Initialise TaskController
    TaskController task_controller;

    // Add sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 1, 2, 10, 2, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 2, 1, 10, 3, 0));

    // Create simulation engine with all scheduler
    SimulationEngine engine(
        "v1-Test_CompleteAllTasks",
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("v1-Test_SchedulerComparison"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Assertions for completion status
    EXPECT_EQ(completed_tasks.size(), 3);
    for (const auto& task: completed_tasks){
        EXPECT_EQ(task->status, TaskStatus::COMPLETED);
        EXPECT_EQ(task->remaining_time, 0);
        EXPECT_GE(task->finish_time, task->arrival_time);
    }
}

TEST_P(SchedulerSanityTest, NoTasksCompleted){
    std::cout << "********* No Tasks Completed Scenario *********\n" << std::endl;
    
    // Initialise TaskController
    TaskController task_controller;

    // Add sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 1, 2, 10, 2, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 2, 1, 10, 3, 0));

    // Create simulation engine with all scheduler
    SimulationEngine engine(
        "v1-Test_NoTasksCompleted",
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("v1-Test_SchedulerComparison"),
        1 // max_ticks
    );

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Assertions for completion status
    EXPECT_EQ(completed_tasks.size(), 0);
}

TEST_P(SchedulerSanityTest, SomeTasksCompleted){
    std::cout << "********* Some Tasks Completed Scenario *********\n" << std::endl;
    
    // Initialise TaskController
    TaskController task_controller;

    // Add sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 1, 2, 10, 2, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 2, 1, 10, 3, 0));

    // Create simulation engine with all scheduler
    SimulationEngine engine(
        "v1-Test_SomeTasksCompleted",
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("v1-Test_SchedulerComparison"),
        5 // max_ticks
    );

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Assertions for completion status
    EXPECT_GT(completed_tasks.size(), 0);
}