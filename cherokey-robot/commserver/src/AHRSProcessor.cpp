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
, angleOffsetRoll(0.0f)
, angleOffsetPitch(0.0f)
, angleOffsetYaw(0.0f)
{    
    auto instance = ConfigManager::getInstance();
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Configuration manager instance "
            "is null");
    }
    
    auto algoType = instance->getAHRSAlgorithm();
    auto useMagnetometer = instance->isUseMagnetometer();
    
    switch (algoType)
    {
        case AHRSAlgorithm::Madgwick:
            ahrsImpl = std::unique_ptr<AHRSStrategy>(new MadgwickAHRS(sf,
                    useMagnetometer));
            break;
        case AHRSAlgorithm::Mahony:
            ahrsImpl = std::unique_ptr<AHRSStrategy>(new MahonyAHRS(sf,
                    useMagnetometer));
            break;
        default:
            COMM_EXCEPTION(InternalError, "Unsupported AHRS algorithm");
    }
    
    compassOffsets = instance->getCompassOffsets();
    gyroThreshold = instance->getGyroThreshold();
    softIronMatrix = instance->getSoftIronMatrix();
    
    assert(softIronMatrix.size1() == 3 && softIronMatrix.size2() == 3);
}

AHRSProcessor::~AHRSProcessor() 
{
}

float AHRSProcessor::filterGyroValue(float rawGyroValue, float& offset)
{
    float gyroAngle = (rawGyroValue - offset) / GYROSCOPE_SENSITIVITY;
    
    if (fabs(gyroAngle) < gyroThreshold && gyroAngle != 0.0f)
    {
        offset = 0.9f * offset + 0.1f * rawGyroValue;
        gyroAngle = 0.0f;
    }
    
    return gyroAngle;
}

void AHRSProcessor::updateState(const IMUSensorsData& data, float& roll, 
        float& pitch, float& yaw)
{
    float gyroX = filterGyroValue(data.rawGyroX, gyroOffsetX);
    float gyroY = filterGyroValue(data.rawGyroY, gyroOffsetY);
    float gyroZ = filterGyroValue(data.rawGyroZ, gyroOffsetZ);
    
    IMUSensorsData processedData = { 0 };
    processedData.rawGyroX = gyroX * M_PI / 180;
    processedData.rawGyroY = gyroY * M_PI / 180;
    processedData.rawGyroZ = gyroZ * M_PI / 180;
    processedData.rawAccelX = data.rawAccelX;
    processedData.rawAccelY = data.rawAccelY;
    processedData.rawAccelZ = data.rawAccelZ;
    
    if (ahrsImpl->isMagnetometerUsed())
    {
        float BcX = data.rawCompassX - compassOffsets.V_x;
        float BcY = data.rawCompassY - compassOffsets.V_y;
        float BcZ = data.rawCompassZ - compassOffsets.V_z;
        
        processedData.rawCompassX = softIronMatrix(0, 0) * BcX +
                softIronMatrix(0, 1) * BcY + softIronMatrix(0, 2) * BcZ;
        processedData.rawCompassY = softIronMatrix(1, 0) * BcX +
                softIronMatrix(1, 1) * BcY + softIronMatrix(1, 2) * BcZ;
        processedData.rawCompassZ = softIronMatrix(2, 0) * BcX +
                softIronMatrix(2, 1) * BcY + softIronMatrix(2, 2) * BcZ;
    }

    ahrsImpl->updateState(processedData, roll, pitch, yaw);

    roll = roll * 180 / M_PI - angleOffsetRoll;
    pitch = pitch * 180 / M_PI - angleOffsetPitch;
    yaw = yaw * 180 / M_PI - angleOffsetYaw;
}

void AHRSProcessor::setGyroOffsets(float offX, float offY, float offZ)
{
    gyroOffsetX = offX;
    gyroOffsetY = offY;
    gyroOffsetZ = offZ;
}

void AHRSProcessor::setAngleOffsets(float ofsRoll, float ofsPitch, 
        float ofsYaw)
{
    angleOffsetPitch = ofsPitch;
    angleOffsetRoll = ofsRoll;
    angleOffsetYaw = ofsYaw;
}
