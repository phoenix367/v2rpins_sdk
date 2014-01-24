/* 
 * File:   ComplementaryFilter.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 25, 2014, 12:50 AM
 */

#include <cmath>
#include "ComplementaryFilter.hpp"

const float ComplementaryFilter::GYROSCOPE_SENSITIVITY = 14.375;

ComplementaryFilter::ComplementaryFilter(float factor) 
: alphaFactor(factor)
{
}

ComplementaryFilter::~ComplementaryFilter() 
{
}

void ComplementaryFilter::getAngles(
        float accelX, float accelY, float accelZ,
        float gyroX, float gyroY, float gyroZ, float dt,
        float& pitch, float& roll)
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
} 
