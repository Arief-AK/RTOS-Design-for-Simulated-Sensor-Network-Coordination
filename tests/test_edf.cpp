#include <gtest/gtest.h>
#include <EDFScheduler.hpp>
#include <SimulationEngine.hpp>

TEST(EDFSchedulerSimulationTest, EarliestDeadlineTaskOrder) {
    std::cout << "********* Earliest Deadline Task Scenario *********\n" << std::endl;

    // Initialise TaskController
    TaskController task_controller;

    // Add sample tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::APERIODIC, 0, 3, 12, 0, 0)); // abs deadline = 12
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 2, 8, 0, 0));  // abs deadline = 8 -> earliest
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::APERIODIC, 0, 4, 10, 0, 0)); // abs deadline = 10

    // Create simulation engine with EDFScheduler
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        std::make_unique<EDFScheduler>(),
        std::make_shared<ConsoleLogger>("Test_EDFScheduler"),
        10 // max_ticks
    );

    // Run the simulation
    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Assertions for completion status
    for (const auto& task : completed_tasks) {
        EXPECT_GE(task->finish_time, task->arrival_time);
        EXPECT_EQ(task->status, TaskStatus::COMPLETED);
        EXPECT_EQ(task->remaining_time, 0);
    }

    // Assert the execution order based on earliest deadline
    EXPECT_EQ(completed_tasks[0]->task_id, 2); // Task with deadline 8
    EXPECT_EQ(completed_tasks[1]->task_id, 3); // Task with deadline 10
    EXPECT_EQ(completed_tasks[2]->task_id, 1); // Task with deadline 12
}

TEST(EDFSchedulerSimulationTest, PeriodicTaskScheduling) {
    std::cout << "********* Periodic Task Scheduling Test *********\n" << std::endl;

    TaskController task_controller;

    // Add periodic tasks with different periods and deadlines
    // ID, Type, Period, Execution, Deadline, Priority, Arrival
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 10, 2, 8, 0, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::PERIODIC, 15, 3, 12, 0, 0));

    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        std::make_unique<EDFScheduler>(),
        std::make_shared<ConsoleLogger>("Test_EDFPeriodicTasks"),
        20  // Run for multiple periods
    );

    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Verify periodic execution
    EXPECT_GE(completed_tasks.size(), 4);  // Should complete multiple instances
    EXPECT_EQ(completed_tasks[0]->task_id, 1);  // First task has earlier deadline
}

TEST(EDFSchedulerSimulationTest, MixedTaskTypes) {
    std::cout << "********* Mixed Task Types Test *********\n" << std::endl;

    TaskController task_controller;

    // Mix of periodic, aperiodic, and sporadic tasks
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 8, 2, 6, 0, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 3, 10, 0, 2));
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::SPORADIC, 0, 2, 8, 0, 4));

    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        std::make_unique<EDFScheduler>(),
        std::make_shared<ConsoleLogger>("Test_EDFMixedTasks"),
        20
    );

    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    EXPECT_GE(completed_tasks.size(), 3);
    // Verify deadline meeting
    for (const auto& task : completed_tasks) {
        EXPECT_LE(task->finish_time, task->arrival_time + task->deadline);
    }
}

TEST(EDFSchedulerSimulationTest, DeadlineMiss) {
    std::cout << "********* Deadline Miss Test *********\n" << std::endl;

    TaskController task_controller;

    // Overload the system to force deadline misses
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 5, 3, 4, 0, 0));
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::PERIODIC, 5, 3, 4, 0, 0));

    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        std::make_unique<EDFScheduler>(),
        std::make_shared<ConsoleLogger>("Test_EDFDeadlineMiss"),
        15
    );

    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Verify that some deadlines were missed
    bool deadline_missed = false;
    for (const auto& task : completed_tasks) {
        if (task->finish_time > task->arrival_time + task->deadline) {
            deadline_missed = true;
            break;
        }
    }
    EXPECT_TRUE(deadline_missed);
}