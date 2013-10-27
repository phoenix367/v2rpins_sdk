/* 
 * File:   i2c-sensor.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 27, 2013, 5:34 PM
 */

#ifndef I2C_SENSOR_HPP
#define	I2C_SENSOR_HPP

#include <string>
#include <inttypes.h>

#include "global.hpp"

namespace sn
{
    class I2CSensor
    {
    public:
        static const std::string I2C_BASE_FOLDER;
        
    public:
        std::string getName();
        uint16_t getId();
        virtual SENSOR_DEVICE getSensorDevice() = 0;
        
    protected:
        void doCommand(const std::string& file,
                std::string& command);
    protected:
        std::string name;
        uint16_t id;
    };
}

#endif	/* I2C_SENSOR_HPP */
