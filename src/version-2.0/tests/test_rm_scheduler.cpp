#include <gtest/gtest.h>
#include <RMScheduler.hpp>
#include <TaskControlBlock.hpp>
#include <vector>

// Helper to create a task for RM (period = deadline)
std::unique_ptr<TaskControlBlock> make_rm_task(int id, int arrival, int comp, int period) {
    return std::make_unique<TaskControlBlock>(id, arrival, comp, period, TaskCriticality::HARD, 1);
}

TEST(RMSchedulerSanity, PicksTaskWithShortestPeriod) {
    RMScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_rm_task(0, 0, 2, 20)); // Lower priority (longer period)
    tasks.push_back(make_rm_task(1, 0, 2, 10)); // Higher priority (shorter period)
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 1); // Task 1 has shorter period
}

TEST(RMSchedulerSanity, IgnoresNotReadyTasks) {
    RMScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_rm_task(0, 0, 2, 10)); // Ready
    tasks.push_back(make_rm_task(1, 5, 2, 5));  // Not arrived yet
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 0); // Only task 0 is ready
}

TEST(RMSchedulerSanity, ReturnsNullIfNoReadyTask) {
    RMScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_rm_task(0, 5, 2, 10)); // Not arrived
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    EXPECT_EQ(selected, nullptr);
}

TEST(RMSchedulerSanity, PicksAmongReadyTasksWithSamePeriod) {
    RMScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_rm_task(0, 0, 2, 10));
    tasks.push_back(make_rm_task(1, 0, 2, 10));
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    // Tie-breaking: should pick the first in the list
    EXPECT_EQ(selected->getTaskId(), 0);
}

TEST(RMSchedulerSanity, PicksTaskWithShortestPeriodEvenIfOthersArrivedEarlier) {
    RMScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_rm_task(0, 0, 2, 20)); // Arrived earlier, longer period
    tasks.push_back(make_rm_task(1, 0, 2, 10)); // Arrived at same time, shorter period
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());
    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 1);
}
