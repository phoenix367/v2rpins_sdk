#include "sensors/i2c-sensor.hpp"

namespace sn
{
    std::string I2CSensor::getName()
    {
        return name;
    }
    
    uint16_t I2CSensor::getId()
    {
        return id;
    }
}
