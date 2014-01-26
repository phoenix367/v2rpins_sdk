/* 
 * File:   ComplementaryFilter.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 25, 2014, 12:50 AM
 */

#include <iosfwd>


#include <cmath>
#include <iostream>
#include <fstream>
#include "ComplementaryFilter.hpp"

const float ComplementaryFilter::GYROSCOPE_SENSITIVITY = 14.375;

std::ofstream fileStream;

ComplementaryFilter::ComplementaryFilter(float factor) 
: alphaFactor(factor)
{
    init();
    
    fileStream.open("compass_data.txt");
}

ComplementaryFilter::~ComplementaryFilter() 
{
    fileStream.close();
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
    roll -= gyroY / GYROSCOPE_SENSITIVITY * dt;    // Angle around the Y-axis
 
	// Turning around the X axis results in a vector on the Y-axis
    pitchAcc = atan2(accelY, sqrt(a_x2 + a_z2)) * 180 / M_PI;
    pitch = pitch * alphaFactor + pitchAcc * (1.0f - alphaFactor);
 
	// Turning around the Y axis results in a vector on the X-axis
    rollAcc = atan2(accelX, sqrt(a_y2 + a_z2)) * 180 / M_PI;
    roll = roll * alphaFactor + rollAcc * (1.0f - alphaFactor);
    
    float theta = pitch * M_PI / 180;
    float phi = roll * M_PI / 180;
    
    float Xh = compassX * cos(theta) + 
        compassY * sin(theta) * sin(phi) +
        compassZ * sin(theta) * cos(phi);
    float Yh = compassZ * sin(phi) - compassY * cos(phi);
    
    
    Xh = compassX + 111.4397;
    Yh = compassY + 162.1687;
    
    float yawCompass = atan2(Yh, Xh) * 180 / M_PI;
    
    
    //fileStream << compassX << " " << compassY << " " <<
    //        compassZ << std::endl;
    std::cout << Xh << " " << Yh << " " << yawCompass << std::endl;
    yaw = yaw * alphaFactor + (1.0f - alphaFactor) * yawCompass;
    
    rollOut = roll;
    pitchOut = pitch;
    yawOut = yaw;
} 
