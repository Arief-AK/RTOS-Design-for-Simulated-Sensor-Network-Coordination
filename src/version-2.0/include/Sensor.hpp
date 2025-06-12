#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <cstdint>
#include <random>

class Sensor{
public:
    explicit Sensor(uint8_t id);

    int read_value() const;

private:
    uint8_t m_id;
};

#endif // SENSOR_HPP