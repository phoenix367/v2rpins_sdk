#include "sensors/adxl345-sensor.hpp"

namespace sn
{
    SENSOR_DEVICE ADXL345Sensor::getSensorDevice()
    {
        return SENSOR_DEVICE::ADXL345;
    }
}
