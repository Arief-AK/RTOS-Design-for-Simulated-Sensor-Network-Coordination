#ifndef JSON_LOGGER_HPP
#define JSON_LOGGER_HPP

#include <Logger.hpp>
#include <filesystem>
// #include <nlohmann/json.hpp>

class JSONLogger : public Logger
{
public:
    JSONLogger(const std::string& filename);
};

#endif // JSON_LOGGER_HPP