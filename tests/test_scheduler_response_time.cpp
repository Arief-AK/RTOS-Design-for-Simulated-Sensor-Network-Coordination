#include <gtest/gtest.h>
#include <RoundRobinScheduler.hpp>
#include <PriorityScheduler.hpp>
#include <EDFScheduler.hpp>
#include <SimulationEngine.hpp>

class SchedulerResponseTimeTest : public ::testing::TestWithParam<std::function<std::unique_ptr<Scheduler>()>> {
protected:
    void checkResponseTime(const std::shared_ptr<TaskControlBlock>& task, int expected_min, int expected_max);
};

// Instantiate the test with different schedulers
INSTANTIATE_TEST_SUITE_P(SchedulerTypes, SchedulerResponseTimeTest,::testing::Values(
    []() {return std::make_unique<RoundRobinScheduler>(2); },
    []() {return std::make_unique<PriorityScheduler>(); },
    []() {return std::make_unique<EDFScheduler>(); }
));

void SchedulerResponseTimeTest::checkResponseTime(const std::shared_ptr<TaskControlBlock>& task, int expected_min, int expected_max) {
    int response_time = task->finish_time - task->arrival_time;
    EXPECT_GE(response_time, expected_min) 
        << "Task " << task->task_id << " completed too quickly. Response time: " << response_time;
    EXPECT_LE(response_time, expected_max) 
        << "Task " << task->task_id << " took too long. Response time: " << response_time;
}

TEST_P(SchedulerResponseTimeTest, ResponseTime) {
    std::cout << "********* Scheduler Response Time *********\n" << std::endl;
    
    TaskController task_controller;

    // Create a mix of tasks with different characteristics
    // ID, Type, Period, Execution, Deadline, Priority, Arrival
    task_controller.addTask(std::make_shared<TaskControlBlock>(1, TaskType::PERIODIC, 10, 3, 8, 2, 0));   // High priority periodic
    task_controller.addTask(std::make_shared<TaskControlBlock>(2, TaskType::APERIODIC, 0, 2, 6, 1, 1));  // Medium priority aperiodic
    task_controller.addTask(std::make_shared<TaskControlBlock>(3, TaskType::SPORADIC, 0, 4, 10, 3, 2));  // Low priority sporadic
    
    SimulationEngine engine(
        std::make_unique<TaskController>(task_controller),
        GetParam()(),
        std::make_shared<ConsoleLogger>("Test_SchedulerResponseTime"),
        20  // Increased simulation time for periodic task
    );

    engine.run();
    auto completed_tasks = engine.getCompletedTasks();

    // Basic completion checks
    ASSERT_GE(completed_tasks.size(), 3) << "Not all tasks completed";
    
    // Verify response times based on scheduler type
    auto scheduler_name = GetParam()()->getName();
    
    for (const auto& task : completed_tasks) {
        // Common assertions for all schedulers
        EXPECT_GT(task->finish_time - task->arrival_time, 0) << "Invalid response time";
        
        if (scheduler_name == "RoundRobinScheduler") {
            // RR should have predictable response times based on quantum and execution time
            checkResponseTime(task, task->execution_time, task->execution_time * 3);
        }
        else if (scheduler_name == "PriorityScheduler") {
            if (task->priority == 1) {  // Highest priority (Task 2)
                // Highest priority task should complete within execution time + arrival time
                checkResponseTime(task, task->execution_time, task->execution_time + task->arrival_time);
            } else if (task->priority == 2) {  // Medium priority (Task 1)
                // May be delayed by highest priority task
                checkResponseTime(task, task->execution_time, task->execution_time + 3);
            } else if (task->priority == 3) {  // Lowest priority (Task 3)
                // Lower priority tasks can take longer but must meet deadline
                checkResponseTime(task, task->execution_time, std::min(task->deadline, task->execution_time + 5));
            }
        }
        else if (scheduler_name == "EDFScheduler") {
            // Tasks should complete based on their deadlines
            checkResponseTime(task, task->execution_time, task->deadline - 1);
        }
        
        std::cout << "Task " << task->task_id 
                  << " (Type: " << static_cast<int>(task->task_type)
                  << ", Priority: " << task->priority 
                  << ") Response Time: " << task->finish_time - task->arrival_time
                  << " ticks" << std::endl;
    }
}