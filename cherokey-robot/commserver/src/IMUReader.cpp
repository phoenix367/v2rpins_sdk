/* 
 * File:   IMUReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 11, 2014, 1:41 AM
 */

#include "IMUReader.hpp"
#include "Exceptions.hpp"
#include "sensors.pb.h"
#include "madgwik_ahrs.h"
#include "ConfigManager.hpp"
#include "AHRSProcessor.hpp"

#include <linux/i2c-dev.h>
#include <linux/swab.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cmath>
#include <fstream>

#define I2CDEV                      "/dev/i2c-1"

#define ADXL345_I2C_ADDR            0x53
#define HMC5883L_I2C_ADDR           0x1E
#define ITG3200_I2C_ADDR            0x68
#define BMA180_I2C_ADDR             0x40

namespace cs = cherokey::sensors;

#define WRITE_SENSORS_DATA          0

IMUReader::IMUReader() 
{
    if ((file = open(I2CDEV, O_RDWR)) < 0) 
    {
        COMM_EXCEPTION(InternalError, "Can't open I2C device");
    }
    
    initThread();
}

IMUReader::~IMUReader() 
{    
    stopThread();
    close(file);
}

void IMUReader::run()
{
    initSensors();
    
    cs::SensorData sensorMessage;

    sensorMessage.set_sensor_id(2);
    sensorMessage.set_sensor_desc("IMU");
    auto values = sensorMessage.mutable_sensor_values();

    auto gyroData = values->Add();
        
    gyroData->set_associated_name("gyro_data");
    gyroData->set_data_type(cs::COORD_3D);
    auto gyroCoords = gyroData->mutable_coord_value();

    int64_t loopCount = 0;
    GyroState gyroState = { 0 };
    bool calibration = true;
    
    int64_t calibrationDelay = 500;
    AHRSProcessor ahrsProcessor(100);
    
    auto instance = ConfigManager::getInstance();
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Can't get instance of "
            "configuration manager");
    }
    
    float pitch, roll, yaw;
    float offsetRoll = 0.0f, offsetPitch = 0.0f, offsetYaw = 0.0f;
    
    auto delayTime = std::chrono::milliseconds(10);
    auto t = std::chrono::high_resolution_clock::now();
    
#if WRITE_SENSORS_DATA
    std::ofstream stream("raw_data.txt");
#endif

    while (!stopVariable)
    {
        IMUSensorsData sensorsData = { 0 };
        readSensors(sensorsData, gyroState, calibration);
        
        std::this_thread::sleep_until(t + delayTime);
        t += delayTime;
        
#if WRITE_SENSORS_DATA
        stream << sensorsData.rawAccelX << " " << sensorsData.rawAccelY <<
                " " << sensorsData.rawAccelZ << " " << sensorsData.rawGyroX << 
                " " << sensorsData.rawGyroY << " " << sensorsData.rawGyroZ <<
                std::endl;
#endif
        
        if (!calibration)
        {
            ahrsProcessor.updateState(sensorsData, roll, pitch, yaw);
        }
        else
        {
            float tmpRoll, tmpPitch, tmpYaw;
            IMUSensorsData tmpData = { 0 };
            
            tmpData.rawAccelX = sensorsData.rawAccelX;
            tmpData.rawAccelY = sensorsData.rawAccelY;
            tmpData.rawAccelZ = sensorsData.rawAccelZ;
            
            ahrsProcessor.updateState(tmpData, tmpRoll, tmpPitch, tmpYaw);
            
            offsetRoll += tmpRoll;
            offsetPitch += tmpPitch;
            offsetYaw += tmpYaw;
        }
        
        gyroCoords->set_x(roll);
        gyroCoords->set_y(pitch);
        gyroCoords->set_z(yaw);
        
        int messageSize = sensorMessage.ByteSize();
        std::vector<int8_t> outArray(messageSize);
        sensorMessage.SerializeToArray(&outArray[0], messageSize);

        if (!calibration && loopCount % 10 == 0)
        {
            //std::cout << sensorsData.rawCompassX << " " << sensorsData.rawCompassY << " " <<
            //        sensorsData.rawCompassZ << std::endl;
            sendData(outArray);
        }
        
        loopCount++;
        
        if (loopCount == calibrationDelay)
        {
            calibration = false;
            
            gyroState.offsetX /= calibrationDelay;
            gyroState.offsetY /= calibrationDelay;
            gyroState.offsetZ /= calibrationDelay;
            
            offsetRoll /= calibrationDelay;
            offsetPitch /= calibrationDelay;
            offsetYaw /= calibrationDelay;
            
            ahrsProcessor.setGyroOffsets(gyroState.offsetX, gyroState.offsetY,
                    gyroState.offsetZ);
            ahrsProcessor.setAngleOffsets(offsetRoll, offsetPitch, offsetYaw);
        }
    }

#if WRITE_SENSORS_DATA    
    stream.close();
#endif
}

void IMUReader::writeToDevice(int file, const char * buf, int len) 
{
    if (write(file, buf, len) != len) 
    {
        printf("Can't write to device\n");
    }
}

void IMUReader::selectDevice(int file, int addr, const char * name) 
{
    if (ioctl(file, I2C_SLAVE, addr) < 0) 
    {
        printf("%s not present\n", name);
    }
}

void IMUReader::initSensors()
{
    /* initialize ADXL345 */
    selectDevice(file, ADXL345_I2C_ADDR, "ADXL345");
    writeToDevice(file, "\x2d\x00", 2);
    writeToDevice(file, "\x2d\x10", 2);
    writeToDevice(file, "\x2d\x08", 2);
    writeToDevice(file, "\x31\x00", 2);
    writeToDevice(file, "\x31\x0b", 2);

    /* initialize HMC5883L */
    selectDevice(file, HMC5883L_I2C_ADDR, "HMC5883L");
    writeToDevice(file, "\x02\x00", 2);

    /* initialize ITG3200 */
    selectDevice(file, ITG3200_I2C_ADDR, "ITG3200");
    //writeToDevice(file, "\x16\b00011000", 2);
    writeToDevice(file, "\x3E\x00", 2);
    writeToDevice(file, "\x15\x07", 2);
    writeToDevice(file, "\x16\x1E", 2);
    writeToDevice(file, "\x17\x00", 2);
}

void IMUReader::readSensors(IMUSensorsData& data, GyroState& gyroState,
            bool calibration)
{
    short x, y, z;
    unsigned char buf[16];

    selectDevice(file, HMC5883L_I2C_ADDR, "HMC5883L");
    writeToDevice(file, "\x03", 1);

    if (read(file, buf, 6) != 6) 
    {
        printf("Unable to read from HMC5883L\n");
    }
    else 
    {
        x = __swab16(*(short*) &buf[0]);
        z = __swab16(*(short*) &buf[2]);
        y = __swab16(*(short*) &buf[4]);
        
        data.rawCompassX = x;
        data.rawCompassY = y;
        data.rawCompassZ = z;
    }

    selectDevice(file, ADXL345_I2C_ADDR, "ADXL345");
    writeToDevice(file, "\x32", 1);

    if (read(file, buf, 6) != 6) 
    {
        printf("Unable to read from ADXL345\n");
    }
    else 
    {
        x = buf[1] << 8 | buf[0];
        y = buf[3] << 8 | buf[2];
        z = buf[5] << 8 | buf[4];
        
        data.rawAccelX = x;
        data.rawAccelY = y;
        data.rawAccelZ = z;
    }

    selectDevice(file, ITG3200_I2C_ADDR, "ITG3200");
    writeToDevice(file, "\x1D", 1);

    if (read(file, buf, 6) != 6) 
    {
        printf("Unable to read from ITG3200\n");
    }
    else 
    {
        x = __swab16(*(short*) &buf[0]);
        y = __swab16(*(short*) &buf[2]);
        z = __swab16(*(short*) &buf[4]);
        
        data.rawGyroX = x;
        data.rawGyroY = y;
        data.rawGyroZ = z;
        
        if (calibration)
        {
            gyroState.offsetX += x;
            gyroState.offsetY += y;
            gyroState.offsetZ += z;
        }
    }
}
