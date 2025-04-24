#include <gtest/gtest.h>
#include <TaskController.hpp>
#include <RoundRobinScheduler.hpp>
#include <PriorityScheduler.hpp>
#include <SimulationEngine.hpp>

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

    // Display tasks
    task_controller.displayTasks();

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

TEST(PrioritySchedulerSimulationTest, HighestPriorityTask){
    std::cout << "********* Highest Priority Task Scenario *********\n" << std::endl;

    // Initialise variables
    TaskController task_controller;

    // Create sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 3, 5, 3, 0)); // Last
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 2, 5, 1, 0)); // Highest priority
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 4, 5, 2, 0)); // Second 

    // Display tasks
    task_controller.displayTasks();

    // Initialise the simulation engine
    SimulationEngine engine(std::make_unique<TaskController>(task_controller), std::make_unique<PriorityScheduler>(), 10);

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Iterate through completed tasks
    for (const auto& task : completed_tasks) {
        EXPECT_GE(task->finish_time, task->arrival_time);
        EXPECT_EQ(task->status, TaskStatus::COMPLETED);
        EXPECT_EQ(task->remaining_time, 0);
    }

    // Check order of priority
    EXPECT_EQ(completed_tasks[0]->task_id, 2); // Highest priority task should be completed first
    EXPECT_EQ(completed_tasks[1]->task_id, 3); // Next highest priority task
    EXPECT_EQ(completed_tasks[2]->task_id, 1); // Lowest priority task should be completed last

    // Print statistics
    engine.printStatistics();
}

TEST(PrioritySchedulerSimulationTest, PriorityInversionScenario){
    std::cout << "********* Priority Inversion Scenario *********\n" << std::endl;

    // Initialise variables
    TaskController task_controller;

    // ID, Type, Period, Execution, Deadline, Priority, Arrival
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 5, 10, 1, 1)); // Low priority
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 4, 10, 2, 2)); // Medium priority
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 3, 10, 3, 3)); // High priority

    // Display tasks
    task_controller.displayTasks();

    // Initialise the simulation engine
    SimulationEngine engine(std::make_unique<TaskController>(task_controller), std::make_unique<PriorityScheduler>(), 10);

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Print statistics
    engine.printStatistics();
}