#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

class Logger{
public:
    virtual ~Logger() = default;
    
    virtual void log(const std::string &message) = 0;
    virtual void logToFile(const std::string &message, const std::string &filename) = 0;    
};

#endif // LOGGER_HPP