#include "sensors/bm085-sensor.hpp"

namespace sn
{
    SENSOR_DEVICE BM085Sensor::getSensorDevice()
    {
        return SENSOR_DEVICE::BMP085;
    }
}
