#include <ConsoleLogger.hpp>

ConsoleLogger::ConsoleLogger(std::string name) : m_logger_name(name), m_file_path("../../../logs/") {
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
    std::cout << m_logger_name << ": " << message << std::endl;
}

void ConsoleLogger::logToFile(const std::string &message){
    // Create a default file name
    m_file_name = m_file_path + m_logger_name + ".log";
    
    if(!m_file_stream.is_open()){
        m_file_stream.open(m_file_name, std::ios::app);
        if(!m_file_stream.is_open()){
            std::cerr << "Error opening file: " << m_file_name << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    m_file_stream << m_logger_name << ": " << message << std::endl;
    m_file_stream.close();
}

void ConsoleLogger::logToFile(const std::string &message, const std::string &filename){
    m_file_name = filename + ".log";

    if(!m_file_stream.is_open()){
        m_file_stream.open(m_file_name, std::ios::app);
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
