#ifndef METRICS_COLLECTOR_HPP
#define METRICS_COLLECTOR_HPP

#include <map>
#include <vector>
#include <memory>
#include <cstdint>

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
    void printSummaryReport() const;
    void exportJSONSummary(const std::string reference_name) const;

private:
    struct TaskMetrics{
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