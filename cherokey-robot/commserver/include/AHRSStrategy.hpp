/* 
 * File:   AHRSStrategy.hpp
 * Author: Ivan Gubochkin
 *
 * Created on February 10, 2014, 9:11 PM
 */

#ifndef AHRSSTRATEGY_HPP
#define	AHRSSTRATEGY_HPP

#include "madgwik_ahrs.h"

struct IMUSensorsData;

class AHRSStrategy
{
public:
    AHRSStrategy(float sr, bool um);
    virtual ~AHRSStrategy();
    
    virtual void updateState(const IMUSensorsData& data, QUATERNION& q) = 0;
    bool isMagnetometerUsed();
    
protected:
    float sampleRate;
    bool useMagnetometer;
};

class MadgwickAHRS : public AHRSStrategy
{
public:
    MadgwickAHRS(float sr, bool um);
    virtual ~MadgwickAHRS();
    
    virtual void updateState(const IMUSensorsData& data, QUATERNION& q);
    
private:
    MADGWICK_AHRS_INFO ahrsInfo;
};

class MahonyAHRS : public AHRSStrategy
{
public:
    MahonyAHRS(float sr, bool um);
    virtual ~MahonyAHRS();
    
    virtual void updateState(const IMUSensorsData& data, QUATERNION& q);
    
private:
    MAHONY_AHRS_INFO ahrsInfo;
};

#endif	/* AHRSSTRATEGY_HPP */
