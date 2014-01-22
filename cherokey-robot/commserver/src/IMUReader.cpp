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

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
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
    boost::asio::io_service io;
    auto delayTime = boost::posix_time::milliseconds(10);
    boost::asio::deadline_timer t(io, delayTime);

    initSensors();
    
    cs::SensorData sensorMessage;

    sensorMessage.set_sensor_id(2);
    sensorMessage.set_sensor_desc("IMU");
    auto values = sensorMessage.mutable_sensor_values();
    auto compassData = values->Add();
    auto gyroData = values->Add();
    auto accelData = values->Add();
    
    compassData->set_associated_name("compass_data");
    compassData->set_data_type(cs::REAL);
    
    gyroData->set_associated_name("gyro_data");
    gyroData->set_data_type(cs::COORD_3D);
    auto gyroCoords = gyroData->mutable_coord_value();
    
    accelData->set_associated_name("accel_data");
    accelData->set_data_type(cs::COORD_3D);
    auto accelCoords = accelData->mutable_coord_value();

    int64_t loopCount = 0;
    GyroState gyroState = { 0 };
    AccelState accelState = { 0 };
    bool calibration = true;
    
    int64_t calibrationDelay = 500;
    AHRS_INFO ahrsInfo;
    InitAHRS(100, &ahrsInfo);

    while (!stopVariable)
    {
        IMUSensorsData sensorsData = { 0 };
        readSensors(sensorsData, gyroState, accelState, calibration);
        
        
        t.wait();
        t.expires_at(t.expires_at() + delayTime);
        
        if (!calibration)
        {
            /*
            MadgwickAHRSupdateIMU(
                    sensorsData.rawGyroX - gyroState.offsetX, 
                    sensorsData.rawGyroY - gyroState.offsetY, 
                    sensorsData.rawGyroZ - gyroState.offsetZ, 
                    0, 0, 0,
                    &ahrsInfo);
            quat2Euler(ahrsInfo.q0, ahrsInfo.q1, ahrsInfo.q2, ahrsInfo.q3,
                    sensorsData.gyroX, sensorsData.gyroY, sensorsData.gyroZ);
            */
        }
        
        compassData->set_real_value(sensorsData.compassAngle);
        gyroCoords->set_x(sensorsData.gyroX);
        gyroCoords->set_y(sensorsData.gyroY);
        gyroCoords->set_z(sensorsData.gyroZ);
        
        accelCoords->set_x(sensorsData.accelX);
        accelCoords->set_y(sensorsData.accelY);
        accelCoords->set_z(sensorsData.accelZ);

        int messageSize = sensorMessage.ByteSize();
        std::vector<int8_t> outArray(messageSize);
        sensorMessage.SerializeToArray(&outArray[0], messageSize);

        if (!calibration && loopCount % 10 == 0)
        {
            std::cout << sensorsData.accelX << " " << sensorsData.accelY << " " <<
                    sensorsData.accelZ << " " << sensorsData.gyroX << " " <<
                    sensorsData.gyroY << " " << sensorsData.gyroZ << std::endl;
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
        data.rawComapssY = y;
        data.rawCompassZ = z;
        
        data.compassAngle = getCompassAngle(x, y, z);
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
        else
        {
            float xCentered = x;
            float yCentered = y;
            float zCentered = z;
            
            float x_2 = xCentered * xCentered, y_2 = yCentered * yCentered,
                    z_2 = zCentered * zCentered;
            
            data.accelX = atan(xCentered / sqrt(y_2 + z_2));
            data.accelY = atan(yCentered / sqrt(x_2 + z_2));
            data.accelZ = atan(sqrt(x_2 + y_2) / zCentered);

            data.accelX *= 180 / M_PI;
            data.accelY *= 180 / M_PI;
            data.accelZ *= 180 / M_PI;
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
        else
        {
            float gyroRate = (x - gyroState.offsetX) / 14.375;
            gyroState.angleX += gyroRate / 100;
            
            data.gyroX = gyroState.angleX;
            
            gyroRate = (y - gyroState.offsetY) / 14.375;
            gyroState.angleY += gyroRate / 100;
            data.gyroY = gyroState.angleY;
            
            gyroRate = (z - gyroState.offsetZ) / 14.375;
            gyroState.angleZ += gyroRate / 100;
            data.gyroZ = gyroState.angleZ;
        }
    }
}

float IMUReader::getCompassAngle(short x, short y, short z)
{
    return 0.0;
}

void IMUReader::quat2Euler(float w, float x, float y, float z, float& roll, 
        float& pitch, float& yaw)
{
    float sqw = w * w;
    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;
    roll = atan2(2.f * (x * y + z * w), sqx - sqy - sqz + sqw);
    pitch = asin(-2.f * (x * z - y * w));
    yaw = atan2(2.f * (y * z + x * w), -sqx - sqy + sqz + sqw);
    
    roll *= 180 / M_PI;
    pitch *= 180 / M_PI;
    yaw *= 180 / M_PI;
}
