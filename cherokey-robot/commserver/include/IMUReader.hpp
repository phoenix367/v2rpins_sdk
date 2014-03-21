/* 
 * File:   IMUReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 11, 2014, 1:41 AM
 */

#ifndef IMUREADER_HPP
#define	IMUREADER_HPP

#include "SensorReader.hpp"
#include "CommandSender.hpp"

#include <mutex>

namespace cherokey
{
    namespace sensors
    {
        class SensorData;
    }
}

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

struct EulerAngles
{
    float roll;
    float pitch;
    float yaw;
};

class IMUReader : public SensorReader, public CommandSender
{
private:
    
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
    
    EulerAngles getCurrentAngles();
    
protected:
    virtual void run();
    
private:
    static void writeToDevice(int file, const char * buf, int len);
    static void selectDevice(int file, int addr, const char * name);
    
    void initSensors();
    void readSensors(IMUSensorsData& data, GyroState& gyroState,
            bool calibration);
    void showCalibration(bool bShow);
    void putCurrentAngles(float roll, float pitch, float yaw);
    
private:
    int file;
    EulerAngles currentAngles;
    std::mutex angleMutex;
};

#endif	/* IMUREADER_HPP */
