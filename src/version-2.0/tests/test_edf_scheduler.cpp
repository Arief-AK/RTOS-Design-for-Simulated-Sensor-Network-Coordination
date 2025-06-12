#include <gtest/gtest.h>
#include <EDFScheduler.hpp>
#include <TaskControlBlock.hpp>
#include <vector>
#include <memory>

// Helper to create a task with an absolute deadline
std::unique_ptr<TaskControlBlock> make_edf_task(int id, int arrival, int comp, int abs_deadline) {
    return std::make_unique<TaskControlBlock>(id, arrival, comp, abs_deadline, TaskCriticality::HARD, 1);
}

TEST(EDFSchedulerSanity, PicksEarliestDeadlineFirstAmongReady) {
    EDFScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_edf_task(0, 0, 2, 15)); // Later deadline
    tasks.push_back(make_edf_task(1, 0, 2, 10)); // Earlier deadline
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());

    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 1);
}

TEST(EDFSchedulerSanity, SkipsTasksNotYetArrived) {
    EDFScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_edf_task(0, 5, 2, 15)); // Not ready
    tasks.push_back(make_edf_task(1, 0, 2, 20)); // Ready
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());

    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 1);
}

TEST(EDFSchedulerSanity, ReturnsNullWhenNoTaskReady) {
    EDFScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_edf_task(0, 5, 2, 15)); // Not ready
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());

    auto selected = scheduler.select_next_task(ptrs, 0);
    EXPECT_EQ(selected, nullptr);
}

TEST(EDFSchedulerSanity, TieBreakerIsFirstInList) {
    EDFScheduler scheduler;
    std::vector<std::unique_ptr<TaskControlBlock>> tasks;
    tasks.push_back(make_edf_task(0, 0, 2, 10));
    tasks.push_back(make_edf_task(1, 0, 2, 10)); // Same deadline
    std::vector<TaskControlBlock*> ptrs;
    for (auto& t : tasks) ptrs.push_back(t.get());

    auto selected = scheduler.select_next_task(ptrs, 0);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->getTaskId(), 0);
}
