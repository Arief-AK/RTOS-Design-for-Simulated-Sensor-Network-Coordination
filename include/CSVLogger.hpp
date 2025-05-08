#ifndef CSV_LOGGER_HPP
#define CSV_LOGGER_HPP

#include <filesystem>
#include <Logger.hpp>
#include <MetricsCollector.hpp>

class CSVLogger : public Logger
{
public:
    CSVLogger(std::shared_ptr<MetricsCollector> collector, const std::string& name="CSV_Logger");
    ~CSVLogger();

    void log(const std::string &message) override;
    void logToFile(const std::string &message) override;
    void logToFile(const std::string &message, const std::string &filename) override;

    void exportToCSV(const std::string& filename);

    std::string getLoggerName() const override;

private:
    std::shared_ptr<MetricsCollector> m_collector;
    std::string m_logger_name;
    std::string m_file_path;
    std::ofstream m_file_stream;

};


#endif // CSV_LOGGER_HPP