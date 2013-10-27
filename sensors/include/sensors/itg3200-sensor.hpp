/* 
 * File:   itg3200-sensor.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 27, 2013, 6:35 PM
 */

#ifndef ITG3200_SENSOR_HPP
#define	ITG3200_SENSOR_HPP

#include "i2c-sensor.hpp"


namespace sn
{
    class ITG3200Sensor : public I2CSensor
    {
    public:
        virtual SENSOR_DEVICE getSensorDevice();
    };
}

#endif	/* ITG3200_SENSOR_HPP */
