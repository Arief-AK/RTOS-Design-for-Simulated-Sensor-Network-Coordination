#include <gtest/gtest.h>
#include <PriorityScheduler.hpp>
#include <TaskControlBlock.hpp>
#include <vector>

// Helper to create a task with priority
std::unique_ptr<TaskControlBlock> make_priority_task(int id, int arrival, int comp, int deadline, int priority) {
    auto t = std::make_unique<TaskControlBlock>(id, arrival, comp, deadline, TaskCriticality::HARD, 1);
    t->setPriority(priority);
    return t;
}

TEST(PrioritySchedulerSanity, PicksHighestPriorityReadyTask) {
    PriorityScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_priority_task(0, 0, 2, 10, 2)); // Lower priority
    tasks.push_back(make_priority_task(1, 0, 2, 10, 1)); // Higher priority
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 1); // Task 1 has higher priority (lower value)
}

TEST(PrioritySchedulerSanity, IgnoresNotReadyTasks) {
    PriorityScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_priority_task(0, 0, 2, 10, 1)); // Ready
    tasks.push_back(make_priority_task(1, 5, 2, 10, 0)); // Not arrived yet
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 0); // Only task 0 is ready
}

TEST(PrioritySchedulerSanity, ReturnsNullIfNoReadyTask) {
    PriorityScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_priority_task(0, 5, 2, 10, 1)); // Not arrived
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    EXPECT_EQ(selected, nullptr);
}

TEST(PrioritySchedulerSanity, PicksAmongReadyTasksWithSamePriority) {
    PriorityScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_priority_task(0, 0, 2, 10, 1));
    tasks.push_back(make_priority_task(1, 0, 2, 10, 1));
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    // Tie-breaking: should pick the first in the list
    EXPECT_EQ(selected->getTaskId(), 0);
}

TEST(PrioritySchedulerSanity, PicksTaskWithHighestPriorityEvenIfOthersArrivedEarlier) {
    PriorityScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_priority_task(0, 0, 2, 10, 2)); // Arrived earlier, lower priority
    tasks.push_back(make_priority_task(1, 0, 2, 10, 1)); // Arrived at same time, higher priority
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 1);
}
