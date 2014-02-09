/* 
 * File:   AHRSProcessor.hpp
 * Author: Ivan Gubochkin
 *
 * Created on February 8, 2014, 9:56 PM
 */

#ifndef AHRSPROCESSOR_HPP
#define	AHRSPROCESSOR_HPP

#include "madgwik_ahrs.h"
#include "ConfigManager.hpp"

struct IMUSensorsData;

class AHRSProcessor 
{
public:
    AHRSProcessor(float sf);
    virtual ~AHRSProcessor();
    
    void updateState(const IMUSensorsData& data, float &roll,
        float& pitch, float& yaw);
    void setGyroOffsets(float offX, float offY, float offZ);
    void setAngleOffsets(float ofsRoll, float ofsPitch, float ofsYaw);
    
private:
    float filterGyroValue(float rawGyroValue, float& offset);
    
private:
    static const float GYROSCOPE_SENSITIVITY;    
    
    AHRS_INFO ahrsInfo;
    float gyroOffsetX;
    float gyroOffsetY;
    float gyroOffsetZ;
    
    float angleOffsetRoll;
    float angleOffsetPitch;
    float angleOffsetYaw;
    
    CompassOffsets compassOffsets;
    float gyroThreshold;
};

#endif	/* AHRSPROCESSOR_HPP */
