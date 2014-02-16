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

typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
} QUATERNION;

typedef struct
{
    float x;
    float y;
    float z;
} COORD_3D;

typedef struct 
{
    float sampleFreq;
    float beta;
    QUATERNION Q;
} MADGWICK_AHRS_INFO;

typedef struct 
{
    float sampleFreq;
    float twoKp;
    float twoKi;
    QUATERNION Q;
    
    // integral error terms scaled by Ki
    float integralFBx;
    float integralFBy;
    float integralFBz;
} MAHONY_AHRS_INFO;

//---------------------------------------------------------------------------------------------------
// Function declarations

bool InitMadgwickAHRS(float sampleRate, MADGWICK_AHRS_INFO* ahrs);
void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, 
        float az, float mx, float my, float mz, MADGWICK_AHRS_INFO* ahrs);
void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, 
        float az, MADGWICK_AHRS_INFO* ahrs);

bool InitMahonyAHRS(float sampleRate, MAHONY_AHRS_INFO* ahrs);
void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, 
        float az, float mx, float my, float mz, MAHONY_AHRS_INFO* ahrs);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, 
        float az, MAHONY_AHRS_INFO* ahrs);

bool Quaternion2Euler(const QUATERNION* q, float *phi, float* theta,
        float* psi);
bool QuaternionConj(const QUATERNION* q_in, QUATERNION* q_out);
bool QuaternionProd(const QUATERNION* q_a, const QUATERNION* q_b,
        QUATERNION* q_out);
bool QuaternionRotate(const COORD_3D* coord_in, const QUATERNION* q,
        COORD_3D* coord_out);

#ifdef	__cplusplus
}
#endif

#endif	/* MADGWIK_AHRS_H */
