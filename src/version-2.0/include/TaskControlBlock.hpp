#ifndef TASK_CONTROL_BLOCK_HPP
#define TASK_CONTROL_BLOCK_HPP

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <cstdint>

#include <TaskBehaviour.hpp>

enum class TaskCriticality { HARD, FIRM, SOFT };
enum class TaskStatus { READY, RUNNING, COMPLETED };

class TaskControlBlock{
public:
    TaskControlBlock(
        uint8_t id, uint8_t arrival, uint8_t computation, uint8_t deadline,
        TaskCriticality critical, uint8_t value
    );
    ~TaskControlBlock();

    void updateMetrics(uint8_t current_time);

    // Accessor methods
    std::stringstream get_stats() const;

    uint8_t getTaskId() const;
    uint8_t getArrivalTime() const;
    uint8_t getComputationTime() const;
    uint8_t getAbsDeadline() const;
    uint8_t getRelDeadline() const;
    uint8_t getStartTime() const;
    uint8_t getFinishTime() const;
    uint8_t getResponseTime() const;
    uint8_t getValue() const;
    uint8_t getTardiness() const;
    
    int8_t getLateness() const;
    int8_t getLaxity() const;
    
    TaskStatus getStatus() const;
    TaskCriticality getCriticality() const;
    void setStatus(TaskStatus new_status);

    // Behaviour methods
    void bindBehaviour(TaskBehaviour* behaviour_fn);
    void execute(uint8_t current_time);

private:
    uint8_t task_id;
    uint8_t arrival_time;          // Time when task is ready for execution
    uint8_t computation_time;      // Time required for task execution 
    uint8_t abs_deadline;          // Absolute deadline for task completion
    uint8_t rel_deadline;          // Relative deadline for task completion (difference between abs deadline and arrival time)
    uint8_t start_time;            // Time when task starts execution
    uint8_t finish_time;           // Time when task finishes execution
    uint8_t response_time;         // Time taken for task to respond
    uint8_t value;                 // Value of importance of the task
    uint8_t tardiness;             // Time task stays active after deadline
    
    int8_t lateness;                // Delay of task completion (if task complete before deadline, lateness is negative)
    int8_t laxity;                  // Maximum time can be delayed without missing its deadline

    TaskCriticality criticality;    // Criticality of the task
    TaskStatus status;              // Status of the task (READY, RUNNING, COMPLETED)
    TaskBehaviour* behaviour;       // Binded behaviour of the task
};

#endif // TASK_CONTROL_BLOCK_HPP
