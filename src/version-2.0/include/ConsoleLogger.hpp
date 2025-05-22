#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <Logger.hpp>

class ConsoleLogger : public Logger{
public:
    ConsoleLogger(std::string name="Console_Logger");
    ~ConsoleLogger();

    void log(const std::string &message) override;
    void logToFile(const std::string &message, const std::string &filename) override;

    std::string getLoggerName() const override;

private:
    std::string m_logger_name;      // Name of the logger
    std::string m_file_name;        // Name of the log file
    std::string m_file_path;        // Path to the log file
    std::ofstream m_file_stream;    // File stream for logging to file
};

#endif // CONSOLE_LOGGER_HPP