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
#include "ComplementaryFilter.hpp"

#include <linux/i2c-dev.h>
#include <linux/swab.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define I2CDEV                      "/dev/i2c-1"

#define ADXL345_I2C_ADDR            0x53
#define HMC5883L_I2C_ADDR           0x1E
#define ITG3200_I2C_ADDR            0x68
#define BMA180_I2C_ADDR             0x40

namespace cs = cherokey::sensors;

const float QuatToEulerAccuracy = 0.001;

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
    AccelState accelState = { 0 };
    bool calibration = true;
    
    int64_t calibrationDelay = 100;
    AHRS_INFO ahrsInfo;
    InitAHRS(1, &ahrsInfo);
    
    auto instance = ConfigManager::getInstance();
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Can't get instance of "
            "configuration manager");
    }
    
    ComplementaryFilter filter;
    float pitch, roll, yaw;
    
    auto delayTime = std::chrono::milliseconds(10);
    auto t = std::chrono::high_resolution_clock::now();

    while (!stopVariable)
    {
        IMUSensorsData sensorsData = { 0 };
        readSensors(sensorsData, gyroState, accelState, calibration);
        
        std::this_thread::sleep_until(t + delayTime);
        t += delayTime;
        
        if (!calibration)
        {
//            MadgwickAHRSupdate(
//                    sensorsData.rawGyroX * M_PI / 180 * 0.01, 
//                    sensorsData.rawGyroY * M_PI / 180 * 0.01, 
//                    sensorsData.rawGyroZ * M_PI / 180 * 0.01, 
//                    sensorsData.rawAccelX, 
//                    sensorsData.rawAccelY, 
//                    sensorsData.rawAccelZ,
//                    sensorsData.rawCompassX,
//                    sensorsData.rawCompassY,
//                    sensorsData.rawCompassZ,
//                    &ahrsInfo);
            MadgwickAHRSupdateIMU(
                    (sensorsData.rawGyroX - gyroState.offsetX) * M_PI / 180 * 0.01, 
                    (sensorsData.rawGyroY - gyroState.offsetY) * M_PI / 180 * 0.01, 
                    (sensorsData.rawGyroZ - gyroState.offsetZ) * M_PI / 180 * 0.01, 
                    sensorsData.rawAccelX, 
                    sensorsData.rawAccelY, 
                    sensorsData.rawAccelZ,
                    &ahrsInfo);
            quat2Euler(ahrsInfo.q0, ahrsInfo.q1, ahrsInfo.q2, ahrsInfo.q3,
                    roll, pitch, yaw);
        }
//        
//        filter.getAngles(sensorsData.rawAccelX, sensorsData.rawAccelY,
//                sensorsData.rawAccelZ, sensorsData.rawGyroX,
//                sensorsData.rawGyroY, sensorsData.rawGyroZ, 
//                sensorsData.rawCompassX, sensorsData.rawCompassY,
//                sensorsData.rawCompassZ,
//                0.01f, pitch, roll, yaw);
        
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
            
            accelState.offsetX /= calibrationDelay;
            accelState.offsetY /= calibrationDelay;
            accelState.offsetZ /= calibrationDelay;
        }
    }
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
            AccelState& accelState, bool calibration)
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


      /*
      selectDevice(file, BMA180_I2C_ADDR, "BMA180");
      writeToDevice(file, "\x02", 1);
   
      if (read(file, buf, 6) != 6) {
         printf("Unable to read from BMA180\n");
      }
      else {
         x = buf[1]<<8| buf[0];
         y = buf[3]<<8| buf[2];
         z = buf[5]<<8| buf[4];
         printf("BMA180: x=%d, y=%d, z=%d\n", x, y, z);
      }
      */


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
        
        if (calibration)
        {
            accelState.offsetX += x;
            accelState.offsetY += y;
            accelState.offsetZ += z;
        }
        
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

float IMUReader::getCompassAngle(short x, short y, short z)
{
    return 0.0;
}

void IMUReader::quat2Euler(float q0, float q1, float q2, float q3, float& roll, 
        float& pitch, float& yaw)
{
    float R[3][3];
    
    R[0][0] = 2 * q0 * q0 - 1 + 2 * q1 * q1;
    R[1][0] = 2 * (q1 * q2 - q0 * q3);
    R[2][0] = 2 *(q1 * q3 + q0 * q2);
    R[2][1] = 2 *(q2 * q3 - q0 * q1);
    R[2][2] = 2 * q0 * q0 - 1 + 2 * q3 * q3;
    
    roll = atan2(R[2][1], R[2][2]);
    pitch = -atan(R[2][0] / sqrt(1 - R[2][0] * R[2][0]));
    yaw = atan2(R[1][0], R[0][0]);

//    phi = atan2(R(3,2,:), R(3,3,:) );
//    theta = -atan(R(3,1,:) ./ sqrt(1-R(3,1,:).^2) );
//    psi = atan2(R(2,1,:), R(1,1,:) );
    
//    R(1,1,:) = 2.*q(:,1).^2-1+2.*q(:,2).^2;
//    R(2,1,:) = 2.*(q(:,2).*q(:,3)-q(:,1).*q(:,4));
//    R(3,1,:) = 2.*(q(:,2).*q(:,4)+q(:,1).*q(:,3));
//    R(3,2,:) = 2.*(q(:,3).*q(:,4)-q(:,1).*q(:,2));
//    R(3,3,:) = 2.*q(:,1).^2-1+2.*q(:,4).^2;
    
    roll *= 180 / M_PI;
    pitch *= 180 / M_PI;
    yaw *= 180 / M_PI;
}
