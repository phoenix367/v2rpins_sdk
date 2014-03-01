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
#include "AHRSStrategy.hpp"

struct IMUSensorsData;

class AHRSProcessor 
{
public:
    AHRSProcessor(float sf);
    virtual ~AHRSProcessor();
    
    void updateState(const IMUSensorsData& data, float &roll,
        float& pitch, float& yaw);
    void updateState(const IMUSensorsData& data, QUATERNION& q);

    void setGyroOffsets(float offX, float offY, float offZ);
    void setOffsetQuaternion(const QUATERNION& q);
    
private:
    float filterGyroValue(float rawGyroValue, float& offset);
    void convertQ2Angles(const QUATERNION& q, float& roll, 
        float& pitch, float& yaw);
    
private:
    static const float GYROSCOPE_SENSITIVITY;    
    
    std::unique_ptr<AHRSStrategy> ahrsImpl;
    float gyroOffsetX;
    float gyroOffsetY;
    float gyroOffsetZ;
    
    CompassOffsets compassOffsets;
    float gyroThreshold;
    boost::numeric::ublas::matrix<float> softIronMatrix;
    QUATERNION offsetQ;
};

#endif	/* AHRSPROCESSOR_HPP */
