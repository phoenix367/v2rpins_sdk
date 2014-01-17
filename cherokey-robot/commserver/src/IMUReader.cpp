/* 
 * File:   IMUReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 11, 2014, 1:41 AM
 */

#include "IMUReader.hpp"
#include "Exceptions.hpp"
#include "sensors.pb.h"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <linux/i2c-dev.h>
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
    
    compassData->set_associated_name("compass_data");
    compassData->set_data_type(cs::COORD_3D);
    auto compassCoords = compassData->mutable_coord_value();

    while (!stopVariable)
    {
        IMUSensorsData sensorsData = { 0 };
        readSensors(sensorsData);
        
        t.wait();
        t.expires_at(t.expires_at() + delayTime);
        
        compassCoords->set_x(sensorsData.compassX);
        compassCoords->set_y(sensorsData.compassY);
        compassCoords->set_z(sensorsData.compassZ);
        
        int messageSize = sensorMessage.ByteSize();
        std::vector<int8_t> outArray(messageSize);
        sensorMessage.SerializeToArray(&outArray[0], messageSize);

        sendData(outArray);
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

void IMUReader::readSensors(IMUSensorsData& data)
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
        x = buf[1]<<8| buf[0];
        y = buf[3]<<8| buf[2];
        z = buf[5]<<8| buf[4];
        data.compassX = (90.0 / 256.0) * (float) x;
        data.compassY = (90.0 / 256.0) * (float) y;
        data.compassZ = (90.0 / 256.0) * (float) z;
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
        x = buf[1]<<8| buf[0];
        y = buf[3]<<8| buf[2];
        z = buf[5]<<8| buf[4];
        data.accelX = (90.0 / 256.0) * (float) x;
        data.accelY = (90.0 / 256.0) * (float) y;
        data.accelZ = (90.0 / 256.0) * (float) z;
    }

    selectDevice(file, ITG3200_I2C_ADDR, "ITG3200");
    writeToDevice(file, "\x1D", 1);

    if (read(file, buf, 6) != 6) 
    {
        printf("Unable to read from ITG3200\n");
    }
    else 
    {
        x = buf[0]<<8| buf[1];
        y = buf[2]<<8| buf[3];
        z = buf[4]<<8| buf[5];
        data.gyroX = (90.0 / 256.0) * (float) x;
        data.gyroY = (90.0 / 256.0) * (float) y;
        data.gyroZ = (90.0 / 256.0) * (float) z;
    }
}
