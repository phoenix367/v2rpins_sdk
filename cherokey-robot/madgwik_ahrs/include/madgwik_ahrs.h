/* 
 * File:   madgwik_ahrs.h
 * Author: Ivan Gubochkin
 *
 * Created on January 21, 2014, 10:20 PM
 */

//=====================================================================================================
// Copied from MadgwickAHRS.h with few modifications
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================

#ifndef MADGWIK_AHRS_H
#define	MADGWIK_AHRS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>

typedef struct {
    float sampleFreq;
    float beta;
    float q0;
    float q1;
    float q2;
    float q3;
} AHRS_INFO;

//---------------------------------------------------------------------------------------------------
// Function declarations

void InitAHRS(float sampleRate, AHRS_INFO* ahrs);
void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, 
        float az, float mx, float my, float mz, AHRS_INFO* ahrs);
void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, 
        float az, AHRS_INFO* ahrs);
bool Quaternion2Euler(const AHRS_INFO* ahrs, float *phi, float* theta,
        float* psi);

#ifdef	__cplusplus
}
#endif

#endif	/* MADGWIK_AHRS_H */
