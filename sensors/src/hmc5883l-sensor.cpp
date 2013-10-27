#include "sensors/hmc5883l-sensor.hpp"

namespace sn
{
    SENSOR_DEVICE HMC5883LSensor::getSensorDevice()
    {
        return SENSOR_DEVICE::HMC5843;
    }
}
