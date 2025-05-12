#ifndef METRICS_COLLECTOR_HPP
#define METRICS_COLLECTOR_HPP

#include <CSVLogger.hpp>
#include <JSONLogger.hpp>
#include <ConsoleLogger.hpp>
#include <TaskControlBlock.hpp>

class MetricsCollector
{
public:
    MetricsCollector(const std::string report_name, std::shared_ptr<Logger> logger, bool to_csv = false, bool to_json = false);
    ~MetricsCollector();

    void analyseTaskCompletion(const std::vector<std::shared_ptr<TaskControlBlock>> &task_list, int current_time);
    void reset();

    void incrementContextSwitchCount();
    int getContextSwitchCount() const;

    void incrementCpuIdleTime();
    int getCPUIdleTime() const;
    
    void printReport(bool to_file = false);

    std::vector<std::shared_ptr<TaskControlBlock>> getTaskList() const;

private:
    int m_total_tasks;
    int m_completed_tasks;
    int m_incomplete_tasks;
    int m_deadline_miss_count;
    int m_total_response_time;
    int m_total_turnaround_time;
    int m_context_switch_count;
    int m_cpu_idle_time;
    int m_cpu_utilisation;

    bool m_to_csv;
    bool m_to_json;

    std::string m_report_name;
    std::shared_ptr<Logger> m_logger;
    std::vector<std::shared_ptr<Logger>> m_loggers;
    std::vector<std::shared_ptr<TaskControlBlock>> m_task_list;
};

#endif // METRICS_COLLECTOR_HPP