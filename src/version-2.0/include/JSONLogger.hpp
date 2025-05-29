#ifndef JSON_LOGGER_HPP
#define JSON_LOGGER_HPP

#include <Logger.hpp>
#include <TimeUtils.hpp>
#include <nlohmann/json.hpp>

class JSONLogger : public Logger{
public:
    JSONLogger(std::string name = "JSON_Logger");
    ~JSONLogger();

    void log(const std::string &message) override;
    void logToFile(const std::string &message, const std::string &filename) override;
    void logStructure(const nlohmann::json &entry);
    void flushToFile(const std::string &filename);

    std::string getLoggerName() const override;

private:
    std::string m_logger_name;
    std::string m_file_name;                // Name of the log file
    std::string m_file_path;                // Path to the log file
    std::ofstream m_file_stream;            // File stream for writing logs
    std::vector<nlohmann::json> m_entries;  // Vector to store log entries in JSON format
};

#endif // JSON_LOGGER_HPP