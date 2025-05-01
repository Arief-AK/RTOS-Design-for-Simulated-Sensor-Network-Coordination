#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <Logger.hpp>

class ConsoleLogger : public Logger
{
public:
    ConsoleLogger(std::string name="Console_Logger");
    ~ConsoleLogger();

    void log(const std::string &message) override;
    void logToFile(const std::string &message) override;
    void logToFile(const std::string &message, const std::string &filename) override;

private:
    std::string m_logger_name;
    std::string m_file_name;
    std::ofstream m_file_stream;
};

#endif // CONSOLE_LOGGER_HPP