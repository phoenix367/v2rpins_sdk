/* 
 * File:   ComplementaryFilter.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 25, 2014, 12:50 AM
 */

#ifndef COMPLEMENTARYFILTER_HPP
#define	COMPLEMENTARYFILTER_HPP

class ComplementaryFilter 
{
public:
    ComplementaryFilter(float factor = 0.98f);
    virtual ~ComplementaryFilter();
    
    void getAngles(float accelX, float accelY, float accelZ,
            float gyroX, float gyroY, float gyroZ, float dt,
            float& pitch, float& roll);
private:
    static const float GYROSCOPE_SENSITIVITY;
    
    float alphaFactor;
};

#endif	/* COMPLEMENTARYFILTER_HPP */
