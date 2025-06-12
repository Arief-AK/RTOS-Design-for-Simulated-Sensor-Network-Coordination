#include <TimeUtils.hpp>

std::string getCurrentSysTime()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") 
        << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}
