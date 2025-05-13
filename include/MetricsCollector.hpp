#ifndef METRICS_COLLECTOR_HPP
#define METRICS_COLLECTOR_HPP

#include <Metrics.hpp>
#include <CSVLogger.hpp>
#include <JSONLogger.hpp>
#include <ConsoleLogger.hpp>
#include <TaskControlBlock.hpp>

class MetricsCollector
{
public:
    MetricsCollector(const std::string report_name, const std::string scheduler_name, std::shared_ptr<Logger> logger, bool to_csv = false, bool to_json = true);
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
    bool m_to_csv;
    bool m_to_json;

    Metrics m_metrics;

    std::string m_report_name;
    std::string m_scheduler_name;
    std::shared_ptr<Logger> m_logger;
    std::vector<std::shared_ptr<Logger>> m_loggers;
    std::vector<std::shared_ptr<TaskControlBlock>> m_task_list;
};

#endif // METRICS_COLLECTOR_HPP