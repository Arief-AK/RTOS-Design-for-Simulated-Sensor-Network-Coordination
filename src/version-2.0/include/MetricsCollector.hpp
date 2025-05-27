#ifndef METRICS_COLLECTOR_HPP
#define METRICS_COLLECTOR_HPP

#include <map>
#include <vector>
#include <memory>

#include <TaskControlBlock.hpp>
#include <nlohmann/json.hpp>
#include <ConsoleLogger.hpp>
#include <JSONLogger.hpp>

class MetricsCollector{
public:
    MetricsCollector(
        std::unique_ptr<Logger> logger = std::make_unique<ConsoleLogger>("Merics_Collector", true)
    );
    ~MetricsCollector();

    void recordTaskMetrics(const TaskControlBlock& task);
    void generateSummaryReport() const;
    void exportJSONSummary() const;

private:
    struct TaskMetrics{
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
        std::string criticality;    // Criticality of the task
        std::string status;              // Status of the task (READY, RUNNING, COMPLETED)
    };

    std::vector<TaskMetrics> m_task_data;

    // Aggregate statistics
    float m_avg_response_time;
    float m_avg_tardiness;
    float m_deadline_miss_rate;

    std::unique_ptr<Logger> m_logger;
    std::unique_ptr<JSONLogger> m_json_logger;
};

#endif // METRICS_COLLECTOR_HPP