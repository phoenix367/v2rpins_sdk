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
        float compassAngle;
        
        float accelX;
        float accelY;
        float accelZ;
        
        float gyroX;
        float gyroY;
        float gyroZ;
        
        float rawCompassX;
        float rawComapssY;
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
        float angleX;
        float angleY;
        float angleZ;
        
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
            bool calibration);
    float getCompassAngle(short x, short y, short z);
    void quat2Euler(float w, float x, float y, float z,
        float& roll, float& pitch, float& yaw);
    
private:
    int file;
};

#endif	/* IMUREADER_HPP */
