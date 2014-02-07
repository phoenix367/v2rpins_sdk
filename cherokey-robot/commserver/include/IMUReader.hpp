/* 
 * File:   IMUReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 11, 2014, 1:41 AM
 */

#ifndef IMUREADER_HPP
#define	IMUREADER_HPP

#include "SensorReader.hpp"

namespace cherokey
{
    namespace sensors
    {
        class SensorData;
    }
}

class IMUReader : public SensorReader
{
private:
    struct IMUSensorsData
    {
        float rawCompassX;
        float rawCompassY;
        float rawCompassZ;
        
        float rawAccelX;
        float rawAccelY;
        float rawAccelZ;
        
        float rawGyroX;
        float rawGyroY;
        float rawGyroZ;
    };
    
    struct GyroState
    {
        float offsetX;
        float offsetY;
        float offsetZ;
    };
    
    struct AccelState
    {
        float offsetX;
        float offsetY;
        float offsetZ;
    };

public:
    IMUReader();
    virtual ~IMUReader();
    
protected:
    virtual void run();
    
private:
    static void writeToDevice(int file, const char * buf, int len);
    static void selectDevice(int file, int addr, const char * name);
    
    void initSensors();
    void readSensors(IMUSensorsData& data, GyroState& gyroState,
            AccelState& accelState, bool calibration);
    float getCompassAngle(short x, short y, short z);
    void quat2Euler(float q0, float q1, float q2, float q3,
        float& roll, float& pitch, float& yaw);
    
private:
    int file;
};

#endif	/* IMUREADER_HPP */
