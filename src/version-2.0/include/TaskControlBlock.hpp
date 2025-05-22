#ifndef TASK_CONTROL_BLOCK_HPP
#define TASK_CONTROL_BLOCK_HPP

#include <iostream>

enum class TaskType { PERIODIC, APERIODIC };
enum class TaskCriticality { HARD, FIRM, SOFT };
enum class TaskStatus { READY, RUNNING, COMPLETED };

struct TaskControlBlock{
    u_int8_t task_id;
    u_int8_t arrival_time;          // Time when task is ready for execution
    u_int8_t computation_time;      // Time required for task execution 
    u_int8_t abs_deadline;          // Absolute deadline for task completion
    u_int8_t rel_deadline;          // Relative deadline for task completion (difference between abs deadline and arrival time)
    u_int8_t start_time;            // Time when task starts execution
    u_int8_t finish_time;           // Time when task finishes execution
    u_int8_t response_time;         // Time taken for task to respond
    TaskCriticality criticality;    // Criticality of the task
    u_int8_t value;                 // Value of importance of the task
    u_int8_t lateness;              // Delay of task completion (if task complete before deadline, lateness is negative)
    u_int8_t tardiness;             // Time task stays active after deadline
    u_int8_t laxity;                // Maximum time can be delayed without missing its deadline    

    TaskControlBlock(
        u_int8_t id, u_int8_t arrival_time, u_int8_t computation_time,
        u_int8_t abs_deadline, TaskCriticality criticality, u_int8_t value
    ) :
        task_id(id), arrival_time(arrival_time), computation_time(computation_time),
        abs_deadline(abs_deadline), rel_deadline(abs_deadline - arrival_time),
        start_time(0), finish_time(0), response_time(0), criticality(criticality),
        value(value), lateness(0), tardiness(0), laxity(0) {};
};

#endif // TASK_CONTROL_BLOCK_HPP
