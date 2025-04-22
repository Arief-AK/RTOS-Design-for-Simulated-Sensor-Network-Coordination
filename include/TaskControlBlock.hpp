#ifndef TASK_CONTROL_BLOCK_HPP
#define TASK_CONTROL_BLOCK_HPP

#include <iostream>

enum class TaskType { PERIODIC, SPORADIC, APERIODIC };
enum class TaskStatus { READY, RUNNING, BLOCKED, COMPLETED };

struct TaskControlBlock{
    int task_id;
    TaskType task_type;
    int period;
    int execution_time;
    int deadline;
    int priority;
    int arrival_time;
    int remaining_time;
    TaskStatus status;

    // Constructor
    TaskControlBlock(
        int id, TaskType type, int period, int exec_time, int deadline,
        int priority, int arr_time
    ):
        task_id(id),
        task_type(type),
        period(period),
        execution_time(exec_time),
        deadline(deadline),
        priority(priority),
        arrival_time(arr_time),
        remaining_time(exec_time),
        status(TaskStatus::READY) {}
};

#endif // TASK_CONTROL_BLOCK_HPP