#include <gtest/gtest.h>
#include <TaskController.hpp>
#include <SimulationEngine.hpp>
#include <RoundRobinScheduler.hpp>

TEST(RoundRobinSchedulerSimulationTest, FiveScenarioTest){
    std::cout << "********* Five Task Scenario *********\n" << std::endl;    
    
    // Initialise variables
    TaskController task_controller;
    
    // Create sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 10, 3, 10, 1, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::SPORADIC, 0, 2, 7, 2, 2));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 1, 0, 3, 4));
    task_controller.addTask(std::make_shared<TaskControlBlock>(4, TaskType::SPORADIC, 0, 2, 6, 4, 5));
    task_controller.addTask(std::make_shared<TaskControlBlock>(5, TaskType::PERIODIC, 10, 2, 10, 5, 6));

    // Intialise the simulation engine
    SimulationEngine engine(std::make_unique<TaskController>(task_controller), std::make_unique<RoundRobinScheduler>(2), 15);

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Iterate through completed tasks
    for (const auto& task : completed_tasks) {
        EXPECT_GE(task->finish_time, task->arrival_time);
        EXPECT_EQ(task->status, TaskStatus::COMPLETED);
        EXPECT_EQ(task->remaining_time, 0);
    }

    // Print statistics
    engine.printStatistics();
}