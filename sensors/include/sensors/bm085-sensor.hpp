/* 
 * File:   bm085-sensor.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 27, 2013, 6:45 PM
 */

#ifndef BM085_SENSOR_HPP
#define	BM085_SENSOR_HPP

#include "i2c-sensor.hpp"

namespace sn
{
    class BM085Sensor : public I2CSensor
    {
    public:
        virtual SENSOR_DEVICE getSensorDevice();
    };
}

#endif	/* BM085_SENSOR_HPP */
