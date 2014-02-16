#include "AHRSStrategy.hpp"
#include "IMUReader.hpp"

AHRSStrategy::AHRSStrategy(float sr, bool um)
: sampleRate(sr)
, useMagnetometer(um)
{
    
}

AHRSStrategy::~AHRSStrategy()
{
    
}

bool AHRSStrategy::isMagnetometerUsed()
{
    return useMagnetometer;
}

void AHRSStrategy::convertQ2Angles(const QUATERNION& q, float& roll, 
        float& pitch, float& yaw)
{
    QUATERNION qConj;
    
    QuaternionConj(&q, &qConj);
    Quaternion2Euler(&qConj, &roll, &pitch, &yaw);
}

MadgwickAHRS::MadgwickAHRS(float sr, bool um)
: AHRSStrategy(sr, um)
{
    InitMadgwickAHRS(sampleRate, &ahrsInfo);
}

MadgwickAHRS::~MadgwickAHRS()
{
    
}

void MadgwickAHRS::updateState(const IMUSensorsData& data, float& roll, 
        float& pitch, float& yaw)
{
    if (useMagnetometer)
    {
        MadgwickAHRSupdate(data.rawGyroX, data.rawGyroY, data.rawGyroZ,
                data.rawAccelX, data.rawAccelY, data.rawAccelZ,
                data.rawCompassX, data.rawCompassY, data.rawCompassZ,
                &ahrsInfo);
    }
    else
    {
        MadgwickAHRSupdateIMU(data.rawGyroX, data.rawGyroY, data.rawGyroZ,
                data.rawAccelX, data.rawAccelY, data.rawAccelZ,
                &ahrsInfo);
    }
    
    convertQ2Angles(ahrsInfo.Q, roll, pitch, yaw);
}

MahonyAHRS::MahonyAHRS(float sr, bool um)
: AHRSStrategy(sr, um)
{
    InitMahonyAHRS(sampleRate, &ahrsInfo);
}

MahonyAHRS::~MahonyAHRS()
{
    
}

void MahonyAHRS::updateState(const IMUSensorsData& data, float& roll, 
        float& pitch, float& yaw)
{
    if (useMagnetometer)
    {
        MahonyAHRSupdate(data.rawGyroX, data.rawGyroY, data.rawGyroZ,
                data.rawAccelX, data.rawAccelY, data.rawAccelZ,
                data.rawCompassX, data.rawCompassY, data.rawCompassZ,
                &ahrsInfo);
    }
    else
    {
        MahonyAHRSupdateIMU(data.rawGyroX, data.rawGyroY, data.rawGyroZ,
                data.rawAccelX, data.rawAccelY, data.rawAccelZ,
                &ahrsInfo);
    }
    
    convertQ2Angles(ahrsInfo.Q, roll, pitch, yaw);
}
