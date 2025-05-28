#ifndef TASK_CONTROL_BLOCK_HPP
#define TASK_CONTROL_BLOCK_HPP

#include <stdint.h>
#include <iostream>
#include <sstream>

#include <TaskBehaviour.hpp>

enum class TaskCriticality { HARD, FIRM, SOFT };
enum class TaskStatus { READY, RUNNING, COMPLETED };

class TaskControlBlock{
public:
    TaskControlBlock(
        u_int8_t id, u_int8_t arrival, u_int8_t computation, u_int8_t deadline,
        TaskCriticality critical, u_int8_t value
    );
    ~TaskControlBlock();

    void updateMetrics(u_int8_t current_time);

    // Accessor methods
    std::stringstream get_stats() const;

    u_int8_t getTaskId() const;
    u_int8_t getArrivalTime() const;
    u_int8_t getComputationTime() const;
    u_int8_t getAbsDeadline() const;
    u_int8_t getRelDeadline() const;
    u_int8_t getStartTime() const;
    u_int8_t getFinishTime() const;
    u_int8_t getResponseTime() const;
    u_int8_t getValue() const;
    u_int8_t getTardiness() const;
    
    int8_t getLateness() const;
    int8_t getLaxity() const;
    
    TaskStatus getStatus() const;
    TaskCriticality getCriticality() const;
    void setStatus(TaskStatus new_status);

    // Behaviour methods
    void bindBehaviour(TaskBehaviour* behaviour_fn);
    void execute(uint8_t current_time);

private:
    u_int8_t task_id;
    u_int8_t arrival_time;          // Time when task is ready for execution
    u_int8_t computation_time;      // Time required for task execution 
    u_int8_t abs_deadline;          // Absolute deadline for task completion
    u_int8_t rel_deadline;          // Relative deadline for task completion (difference between abs deadline and arrival time)
    u_int8_t start_time;            // Time when task starts execution
    u_int8_t finish_time;           // Time when task finishes execution
    u_int8_t response_time;         // Time taken for task to respond
    u_int8_t value;                 // Value of importance of the task
    u_int8_t tardiness;             // Time task stays active after deadline
    
    int8_t lateness;                // Delay of task completion (if task complete before deadline, lateness is negative)
    int8_t laxity;                  // Maximum time can be delayed without missing its deadline

    TaskCriticality criticality;    // Criticality of the task
    TaskStatus status;              // Status of the task (READY, RUNNING, COMPLETED)
    TaskBehaviour* behaviour;       // Binded behaviour of the task
};

#endif // TASK_CONTROL_BLOCK_HPP
