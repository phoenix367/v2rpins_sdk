/* 
 * File:   hmc5883l-sensor.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 27, 2013, 6:57 PM
 */

#ifndef HMC5883L_SENSOR_HPP
#define	HMC5883L_SENSOR_HPP

#include "i2c-sensor.hpp"

namespace sn
{
    class HMC5883LSensor : public I2CSensor
    {
    public:
        virtual SENSOR_DEVICE getSensorDevice();
    };
}

#endif	/* HMC5883L_SENSOR_HPP */
