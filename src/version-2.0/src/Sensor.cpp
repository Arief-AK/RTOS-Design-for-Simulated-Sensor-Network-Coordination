#include <Sensor.hpp>

Sensor::Sensor(uint8_t id) :m_id(id) {}

int Sensor::read_value() const{
    static std::mt19937 rng(m_id);
    std::uniform_int_distribution<int> dist(0, 100);
    return dist(rng);
}
