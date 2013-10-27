#include "sensors/bm085-sensor.hpp"

namespace sn
{
    BM085Sensor::BM085Sensor()
    {
        name = "bmp085";
        id = 0x77;
    }
    
    BM085Sensor::~BM085Sensor()
    {
        
    }
    
    SENSOR_DEVICE BM085Sensor::getSensorDevice()
    {
        return SENSOR_DEVICE::BMP085;
    }
}
