/* 
 * File:   AHRSProcessor.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on February 8, 2014, 9:56 PM
 */

#include "AHRSProcessor.hpp"
#include "IMUReader.hpp"
#include "Exceptions.hpp"

#include <cmath>

const float AHRSProcessor::GYROSCOPE_SENSITIVITY = 14.375;

AHRSProcessor::AHRSProcessor(float sf) 
: gyroOffsetX(0.0f)
, gyroOffsetY(0.0f)
, gyroOffsetZ(0.0f)
{
    InitAHRS(sf, &ahrsInfo);
    
    auto instance = ConfigManager::getInstance();
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Configuration manager instance "
            "is null");
    }
    
    compassOffsets = instance->getCompassOffsets();
}

AHRSProcessor::~AHRSProcessor() 
{
}

void AHRSProcessor::updateState(const IMUSensorsData& data, float& roll, 
        float& pitch, float& yaw)
{
    MadgwickAHRSupdateIMU(
            (data.rawGyroX - gyroOffsetX) * M_PI / 180 /
                GYROSCOPE_SENSITIVITY, 
            (data.rawGyroY - gyroOffsetY) * M_PI / 180 /
                GYROSCOPE_SENSITIVITY, 
            (data.rawGyroZ - gyroOffsetZ) * M_PI / 180 /
                GYROSCOPE_SENSITIVITY, 
            data.rawAccelX, 
            data.rawAccelY, 
            data.rawAccelZ,
            &ahrsInfo);

    Quaternion2Euler(&ahrsInfo, &roll, &pitch, &yaw);

    roll *= 180 / M_PI;
    pitch *= 180 / M_PI;
    yaw *= 180 / M_PI;
}

void AHRSProcessor::setGyroOffsets(float offX, float offY, float offZ)
{
    gyroOffsetX = offX;
    gyroOffsetY = offY;
    gyroOffsetZ = offZ;
}