/* 
 * File:   adxl345-sensor.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 27, 2013, 6:49 PM
 */

#ifndef ADXL345_SENSOR_HPP
#define	ADXL345_SENSOR_HPP

#include "i2c-sensor.hpp"

namespace sn
{
    class ADXL345Sensor : public I2CSensor
    {
    public:
        virtual SENSOR_DEVICE getSensorDevice();
    };
}

#endif	/* ADXL345_SENSOR_HPP */
