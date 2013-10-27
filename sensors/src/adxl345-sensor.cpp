#include "sensors/adxl345-sensor.hpp"

namespace sn
{
    SENSOR_DEVICE ADXL345Sensor::getSensorDevice()
    {
        return SENSOR_DEVICE::ADXL345;
    }
    
    ADXL345Sensor::ADXL345Sensor()
    {
        name = "adxl345";
        id = 0x53;
    }
    
    ADXL345Sensor::~ADXL345Sensor()
    {
        
    }
}
