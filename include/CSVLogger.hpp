#ifndef CSV_LOGGER_HPP
#define CSV_LOGGER_HPP

#include <filesystem>
#include <Logger.hpp>
#include <MetricsCollector.hpp>
#include <TaskControlBlock.hpp>

class CSVLogger : public Logger
{
public:
    CSVLogger(const std::string& name="CSV_Logger");
    ~CSVLogger();

    void setTasks(const std::vector<std::shared_ptr<TaskControlBlock>> &task_list);

    void log(const std::string &message) override;
    void logToFile(const std::string &message) override;
    void logToFile(const std::string &message, const std::string &filename) override;

    void exportToCSV(const std::string& filename);

    std::string getLoggerName() const override;

private:
    std::string m_logger_name;
    std::string m_file_path;
    std::ofstream m_file_stream;
    std::vector<std::shared_ptr<TaskControlBlock>> m_task_list;

};


#endif // CSV_LOGGER_HPP