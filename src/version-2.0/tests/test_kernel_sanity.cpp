#include <gtest/gtest.h>
#include <Kernel.hpp>
#include <RMScheduler.hpp>
#include <TaskControlBlock.hpp>
#include <memory>
#include <vector>

// Dummy logger to suppress output during tests
class DummyLogger : public Logger {
public:
    DummyLogger() = default;
    void log(const std::string&) override {}
    void logToFile(const std::string&, const std::string&) override {}
    std::string getLoggerName() const override { return "Dummy"; }
};

TEST(KernelSanity, ExecutesSingleTask) {
    auto scheduler = std::make_unique<RMScheduler>();
    auto logger = std::make_unique<DummyLogger>();
    Kernel kernel(std::move(scheduler), std::move(logger));
    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 2, 10, TaskCriticality::HARD, 1);
    kernel.addTask(std::move(t1));
    kernel.runPreemptive(5);
    // Check that the task is completed
    // (Assume Kernel exposes m_taskList for test or provide a getter)
    // Here, we check via the pointer in the kernel's task list
    bool all_completed = true;
    for (const auto& t : kernel.getTaskList()) {
        if (!t->isCompleted()) all_completed = false;
    }
    EXPECT_TRUE(all_completed);
}

TEST(KernelSanity, PreemptsLowerPriorityTask) {
    auto scheduler = std::make_unique<RMScheduler>();
    auto logger = std::make_unique<DummyLogger>();
    Kernel kernel(std::move(scheduler), std::move(logger));
    auto t1 = std::make_unique<TaskControlBlock>(0, 0, 6, 20, TaskCriticality::HARD, 1); // Low priority
    auto t2 = std::make_unique<TaskControlBlock>(1, 2, 3, 10, TaskCriticality::FIRM, 1); // High priority
    kernel.addTask(std::move(t1));
    kernel.addTask(std::move(t2));
    kernel.runPreemptive(10);
    // t2 should complete before t1
    const auto& tasks = kernel.getTaskList();
    int t1_finish = -1, t2_finish = -1;
    for (const auto& t : tasks) {
        if (t->getTaskId() == 0) t1_finish = t->getFinishTime();
        if (t->getTaskId() == 1) t2_finish = t->getFinishTime();
    }
    EXPECT_GT(t1_finish, t2_finish); // t2 finishes before t1
}

TEST(KernelSanity, HandlesNoTasks) {
    auto scheduler = std::make_unique<RMScheduler>();
    auto logger = std::make_unique<DummyLogger>();
    Kernel kernel(std::move(scheduler), std::move(logger));
    // Should not crash or throw
    EXPECT_NO_THROW(kernel.runPreemptive(5));
}