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
    auto low = std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 5, 10, 1, 0);    // Low priority
    auto med = std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 4, 10, 2, 2);    // Medium priority
    auto high = std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 3, 10, 3, 4);   // High priority
    
    task_controller.addTask(low);
    task_controller.addTask(med);
    task_controller.addTask(high);

    // Initialise the simulation engine
    SimulationEngine engine(std::make_unique<TaskController>(task_controller), std::make_unique<PriorityScheduler>(), 15);

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Iterate through completed tasks
    for (const auto& task : completed_tasks) {
        EXPECT_GE(task->finish_time, task->arrival_time);
        EXPECT_EQ(task->status, TaskStatus::COMPLETED);
        EXPECT_EQ(task->remaining_time, 0);
    }

    // Verify basic completion
    ASSERT_EQ(completed_tasks.size(), 3) << "Not all tasks completed";

    // Verify priority inversion
    EXPECT_GT(completed_tasks[2]->start_time, completed_tasks[1]->start_time) 
        << "High priority task should be delayed by resource blocking";
    EXPECT_GT(completed_tasks[2]->finish_time - completed_tasks[2]->arrival_time,
              completed_tasks[2]->execution_time + completed_tasks[1]->execution_time)
        << "High priority task should take longer due to priority inversion";

    // Print statistics
    engine.printStatistics();
}