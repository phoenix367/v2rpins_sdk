/* 
 * File:   ComplementaryFilter.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 25, 2014, 12:50 AM
 */

#include <iosfwd>


#include <cmath>
#include <iostream>
#include "ComplementaryFilter.hpp"
#include "Exceptions.hpp"

const float ComplementaryFilter::GYROSCOPE_SENSITIVITY = 14.375;

template <typename T> int sgn(T val) 
{
    return (T(0) < val) - (val < T(0));
}

ComplementaryFilter::ComplementaryFilter(float factor) 
: alphaFactor(factor)
{
    init();
    
    auto instance = ConfigManager::getInstance();
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Can't get instance of "
            "configuration manager");
    }
    
    offsets = instance->getCompassOffsets();
}

ComplementaryFilter::~ComplementaryFilter() 
{
}

void ComplementaryFilter::init()
{
    roll = pitch = yaw = 0.0f;
}

void ComplementaryFilter::getAngles(
        float accelX, float accelY, float accelZ,
        float gyroX, float gyroY, float gyroZ, 
        float compassX, float compassY, float compassZ, float dt,
        float& pitchOut, float& rollOut, float& yawOut)
{
    float pitchAcc, rollAcc;
    
    float a_x2 = accelX * accelX, a_y2 = accelY * accelY,
            a_z2 = accelZ * accelZ;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    pitch += gyroX / GYROSCOPE_SENSITIVITY * dt; // Angle around the X-axis
    roll += gyroY / GYROSCOPE_SENSITIVITY * dt;    // Angle around the Y-axis
    yaw += gyroZ / GYROSCOPE_SENSITIVITY * dt;    // Angle around the Y-axis
 
	// Turning around the X axis results in a vector on the Y-axis
    pitchAcc = atan2(-accelX, sqrt(a_y2 + a_z2)) * 180 / M_PI;
    pitch = pitch * alphaFactor + pitchAcc * (1.0f - alphaFactor);
 
	// Turning around the Y axis results in a vector on the X-axis
    rollAcc = atan2(accelY, sgn(accelZ) * sqrt(a_x2 + a_z2)) * 180 / M_PI;
    roll = roll * alphaFactor + rollAcc * (1.0f - alphaFactor);
    
    float pitchRad = pitchAcc * M_PI / 180;
    float rollRad = rollAcc * M_PI / 180;
    
    float Xcal = compassX - offsets.V_x;
    float Ycal = compassY - offsets.V_y;
    float Zcal = compassZ - offsets.V_z;
    
    float Xh = Xcal * cos(pitchRad) + Ycal * sin(pitchRad) * sin(rollRad) + 
        Zcal * sin(pitchRad) * cos(rollRad);
    float Yh = Zcal * sin(rollRad) - Ycal * cos(rollRad);

    float yawCompass = atan2(Yh, Xh) * 180 / M_PI;
    
    if (yawCompass < 0)
    {
        yawCompass = 360 + yawCompass;
    }
    
    //fileStream << compassX << " " << compassY << " " <<
    //        compassZ << std::endl;
    //std::cout << Xh << " " << Yh << " " << yawCompass << std::endl;
    yaw = yaw * alphaFactor + yawCompass * (1.0f - alphaFactor);
    
    rollOut = roll;
    pitchOut = pitch;
    yawOut = yaw;
} 
