#include <ConsoleLogger.hpp>

ConsoleLogger::ConsoleLogger(std::string name, bool log_to_file) : m_logger_name(name), m_file_path("../../../logs/version-2.0"), m_log_to_file(log_to_file){
    // Create logs directory if it doesn't exist
    std::filesystem::create_directories(m_file_path);
}

ConsoleLogger::~ConsoleLogger()
{
    if (m_file_stream.is_open()) {
        m_file_stream.close();
    }
}

void ConsoleLogger::log(const std::string &message){
    std::cout << " [" << getCurrentSysTime() << "] " << m_logger_name << ": " << message << std::endl;
    if(m_log_to_file)
        logToFile(message, m_logger_name);
}

void ConsoleLogger::logToFile(const std::string &message, const std::string &filename){
    if(!m_file_stream.is_open()){
        m_file_stream.open(m_file_path + "/" + filename + ".log", std::ios::app);
        if(!m_file_stream.is_open()){
            std::cerr << "Error opening file: " << m_file_name << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    m_file_stream << m_logger_name << ": " << message << std::endl;
    m_file_stream.close();
}

std::string ConsoleLogger::getLoggerName() const{
    return m_logger_name;
}