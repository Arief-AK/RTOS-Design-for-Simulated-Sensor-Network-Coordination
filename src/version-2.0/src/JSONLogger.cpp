#include <JSONLogger.hpp>

JSONLogger::JSONLogger(std::string name)
    :m_logger_name(name), m_file_name{}, m_file_path("../../../json/version-2.0") {
    // Create json directory if it doesn't exist
    std::filesystem::create_directories(m_file_path);
    }

JSONLogger::~JSONLogger() {
    if (m_file_stream.is_open()) {
        m_file_stream.close();
    }
}

void JSONLogger::log(const std::string &message){
    std::cout << " [" << getCurrentSysTime() << "] " << m_logger_name << ": " << message << std::endl;
}

void JSONLogger::logToFile(const std::string &message, const std::string &filename) {
    if(!m_file_stream.is_open()){
        m_file_stream.open(m_file_path + "/" + filename + ".log", std::ios::app);
        if(!m_file_stream.is_open()){
            std::cerr << "Error opening file: " << m_file_path << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    m_file_stream << " [" << getCurrentSysTime() << "] " << m_logger_name << ": " << message << std::endl;
    m_file_stream.close();
}

void JSONLogger::logStructure(const nlohmann::json &entry) {
    m_entries.push_back(entry);
}

void JSONLogger::flushToFile(const std::string &filename) {
    if(!m_file_stream.is_open()){
        m_file_stream.open(m_file_path + filename + ".json", std::ios::app);
        if(!m_file_stream.is_open()){
            std::cerr << "Error opening file: " << m_file_path << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    m_file_stream << m_entries << std::endl;
    m_file_stream.close();
}

std::string JSONLogger::getLoggerName() const {
    return m_logger_name;
}