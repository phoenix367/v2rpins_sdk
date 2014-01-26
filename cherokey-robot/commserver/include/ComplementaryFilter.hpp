/* 
 * File:   ComplementaryFilter.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 25, 2014, 12:50 AM
 */

#ifndef COMPLEMENTARYFILTER_HPP
#define	COMPLEMENTARYFILTER_HPP

#include "ConfigManager.hpp"

class ComplementaryFilter 
{
public:
    ComplementaryFilter(float factor = 0.98f);
    virtual ~ComplementaryFilter();
    
    void init();
    void getAngles(float accelX, float accelY, float accelZ,
            float gyroX, float gyroY, float gyroZ, 
            float compassX, float compassY, float compassZ, float dt,
            float& pitchOut, float& rollOut, float& yawOut);
private:
    static const float GYROSCOPE_SENSITIVITY;
    
    float alphaFactor;
    float roll;
    float pitch;
    float yaw;
    
    CompassOffsets offsets;
};

#endif	/* COMPLEMENTARYFILTER_HPP */
