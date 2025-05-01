#ifndef METRICS_COLLECTOR_HPP
#define METRICS_COLLECTOR_HPP

#include <ConsoleLogger.hpp>
#include <TaskControlBlock.hpp>

class MetricsCollector
{
public:
    MetricsCollector(const std::string report_name, std::shared_ptr<Logger> logger);
    ~MetricsCollector();

    void analyseTaskCompletion(const std::vector<std::shared_ptr<TaskControlBlock>> &task_list);
    void reset();
    
    void printReport(bool to_file = false);

private:
    int m_total_tasks;
    int m_completed_tasks;
    int m_deadline_miss_count;
    int m_total_response_time;
    int m_total_turnaround_time;
    int m_context_switch_count;
    int m_cpu_idle_time;
    int m_cpu_utilisation;

    std::string m_report_name;
    std::shared_ptr<Logger> m_logger;

    void incrementContextSwitchCount();
    void incrementCpuIdleTime();
};

#endif // METRICS_COLLECTOR_HPP