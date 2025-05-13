#ifndef JSON_LOGGER_HPP
#define JSON_LOGGER_HPP

#include <Logger.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <TaskControlBlock.hpp>

class JSONLogger : public Logger
{
public:
    JSONLogger(const std::string& name="JSON_Logger");
    ~JSONLogger();

    void setTasks(const std::vector<std::shared_ptr<TaskControlBlock>> &task_list);

    void log(const std::string &message) override;
    void logToFile(const std::string &message) override;
    void logToFile(const std::string &message, const std::string &filename) override;

    void exportToJSON(const std::string& filename, const std::string& sim_name, const std::string& scheduler_name);

    std::string getLoggerName() const override;

private:
    std::string m_logger_name;
    std::string m_file_path;
    std::ofstream m_file_stream;
    std::vector<std::shared_ptr<TaskControlBlock>> m_task_list;

    std::string _getCurrentSystemTime();
};

#endif // JSON_LOGGER_HPP