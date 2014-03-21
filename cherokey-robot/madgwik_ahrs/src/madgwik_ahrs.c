//=====================================================================================================
// MadgwickAHRS.c
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 19/02/2012	SOH Madgwick	Magnetometer measurement is normalised
//
//=====================================================================================================

//---------------------------------------------------------------------------------------------------
// Header files

#include "madgwik_ahrs.h"
#include <math.h>

//---------------------------------------------------------------------------------------------------
// Definitions

#define betaDef		0.1f            // 2 * proportional gain
#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain

//---------------------------------------------------------------------------------------------------
// Function declarations

float invSqrt(float x);

//====================================================================================================
// Functions

bool InitMadgwickAHRS(float sampleRate, MADGWICK_AHRS_INFO* ahrs) 
{
    if (!ahrs) 
    {
        return false;
    }
    
    ahrs->sampleFreq = sampleRate;
    ahrs->beta = betaDef;
    ahrs->Q.q0 = 1.0f;
    ahrs->Q.q1 = 0.0f;
    ahrs->Q.q2 = 0.0f;
    ahrs->Q.q3 = 0.0f;
    
    return true;
}

//---------------------------------------------------------------------------------------------------
// AHRS algorithm update

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, 
        float az, float mx, float my, float mz, MADGWICK_AHRS_INFO* ahrs) 
{
    if (!ahrs)
    {
        return;
    }
    
    float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, 
            _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, 
            q1q3, q2q2, q2q3, q3q3;

    if (!ahrs) {
        return;
    }
    
    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az, ahrs);
		return;
	}

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-ahrs->Q.q1 * gx - ahrs->Q.q2 * gy - ahrs->Q.q3 * gz);
	qDot2 = 0.5f * (ahrs->Q.q0 * gx + ahrs->Q.q2 * gz - ahrs->Q.q3 * gy);
	qDot3 = 0.5f * (ahrs->Q.q0 * gy - ahrs->Q.q1 * gz + ahrs->Q.q3 * gx);
	qDot4 = 0.5f * (ahrs->Q.q0 * gz + ahrs->Q.q1 * gy - ahrs->Q.q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * ahrs->Q.q0 * mx;
		_2q0my = 2.0f * ahrs->Q.q0 * my;
		_2q0mz = 2.0f * ahrs->Q.q0 * mz;
		_2q1mx = 2.0f * ahrs->Q.q1 * mx;
		_2q0 = 2.0f * ahrs->Q.q0;
		_2q1 = 2.0f * ahrs->Q.q1;
		_2q2 = 2.0f * ahrs->Q.q2;
		_2q3 = 2.0f * ahrs->Q.q3;
		_2q0q2 = 2.0f * ahrs->Q.q0 * ahrs->Q.q2;
		_2q2q3 = 2.0f * ahrs->Q.q2 * ahrs->Q.q3;
		q0q0 = ahrs->Q.q0 * ahrs->Q.q0;
		q0q1 = ahrs->Q.q0 * ahrs->Q.q1;
		q0q2 = ahrs->Q.q0 * ahrs->Q.q2;
		q0q3 = ahrs->Q.q0 * ahrs->Q.q3;
		q1q1 = ahrs->Q.q1 * ahrs->Q.q1;
		q1q2 = ahrs->Q.q1 * ahrs->Q.q2;
		q1q3 = ahrs->Q.q1 * ahrs->Q.q3;
		q2q2 = ahrs->Q.q2 * ahrs->Q.q2;
		q2q3 = ahrs->Q.q2 * ahrs->Q.q3;
		q3q3 = ahrs->Q.q3 * ahrs->Q.q3;

		// Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * ahrs->Q.q3 + _2q0mz * ahrs->Q.q2 + mx * q1q1 + 
                _2q1 * my * ahrs->Q.q2 + _2q1 * mz * ahrs->Q.q3 - mx * q2q2 - 
                mx * q3q3;
		hy = _2q0mx * ahrs->Q.q3 + my * q0q0 - _2q0mz * ahrs->Q.q1 + 
                _2q1mx * ahrs->Q.q2 - my * q1q1 + my * q2q2 + 
                _2q2 * mz * ahrs->Q.q3 - my * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * ahrs->Q.q2 + _2q0my * ahrs->Q.q1 + mz * q0q0 + 
                _2q1mx * ahrs->Q.q3 - mz * q1q1 + _2q2 * my * ahrs->Q.q3 - 
                mz * q2q2 + mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - 
                _2bz * ahrs->Q.q2 * (_2bx * (0.5f - q2q2 - q3q3) + 
                _2bz * (q1q3 - q0q2) - mx) + (-_2bx * ahrs->Q.q3 + _2bz * ahrs->Q.q1) * 
                (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + 
                _2bx * ahrs->Q.q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 
                4.0f * ahrs->Q.q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + 
                _2bz * ahrs->Q.q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + 
                (_2bx * ahrs->Q.q2 + _2bz * ahrs->Q.q0) * (_2bx * (q1q2 - q0q3) + 
                _2bz * (q0q1 + q2q3) - my) + (_2bx * ahrs->Q.q3 - _4bz * ahrs->Q.q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 
                4.0f * ahrs->Q.q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + 
                (-_4bx * ahrs->Q.q2 - _2bz * ahrs->Q.q0) * (_2bx * (0.5f - q2q2 - q3q3) + 
                _2bz * (q1q3 - q0q2) - mx) + (_2bx * ahrs->Q.q1 + _2bz * ahrs->Q.q3) * 
                (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + 
                (_2bx * ahrs->Q.q0 - _4bz * ahrs->Q.q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + 
                (-_4bx * ahrs->Q.q3 + _2bz * ahrs->Q.q1) * (_2bx * (0.5f - q2q2 - q3q3) + 
                _2bz * (q1q3 - q0q2) - mx) + (-_2bx * ahrs->Q.q0 + _2bz * ahrs->Q.q2) * 
                (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + 
                _2bx * ahrs->Q.q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= ahrs->beta * s0;
		qDot2 -= ahrs->beta * s1;
		qDot3 -= ahrs->beta * s2;
		qDot4 -= ahrs->beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	ahrs->Q.q0 += qDot1 * (1.0f / ahrs->sampleFreq);
	ahrs->Q.q1 += qDot2 * (1.0f / ahrs->sampleFreq);
	ahrs->Q.q2 += qDot3 * (1.0f / ahrs->sampleFreq);
	ahrs->Q.q3 += qDot4 * (1.0f / ahrs->sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(ahrs->Q.q0 * ahrs->Q.q0 + ahrs->Q.q1 * ahrs->Q.q1 + 
            ahrs->Q.q2 * ahrs->Q.q2 + ahrs->Q.q3 * ahrs->Q.q3);
	ahrs->Q.q0 *= recipNorm;
	ahrs->Q.q1 *= recipNorm;
	ahrs->Q.q2 *= recipNorm;
	ahrs->Q.q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, 
        float az, MADGWICK_AHRS_INFO* ahrs) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

    if (!ahrs) {
        return;
    }
    
	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-ahrs->Q.q1 * gx - ahrs->Q.q2 * gy - ahrs->Q.q3 * gz);
	qDot2 = 0.5f * (ahrs->Q.q0 * gx + ahrs->Q.q2 * gz - ahrs->Q.q3 * gy);
	qDot3 = 0.5f * (ahrs->Q.q0 * gy - ahrs->Q.q1 * gz + ahrs->Q.q3 * gx);
	qDot4 = 0.5f * (ahrs->Q.q0 * gz + ahrs->Q.q1 * gy - ahrs->Q.q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * ahrs->Q.q0;
		_2q1 = 2.0f * ahrs->Q.q1;
		_2q2 = 2.0f * ahrs->Q.q2;
		_2q3 = 2.0f * ahrs->Q.q3;
		_4q0 = 4.0f * ahrs->Q.q0;
		_4q1 = 4.0f * ahrs->Q.q1;
		_4q2 = 4.0f * ahrs->Q.q2;
		_8q1 = 8.0f * ahrs->Q.q1;
		_8q2 = 8.0f * ahrs->Q.q2;
		q0q0 = ahrs->Q.q0 * ahrs->Q.q0;
		q1q1 = ahrs->Q.q1 * ahrs->Q.q1;
		q2q2 = ahrs->Q.q2 * ahrs->Q.q2;
		q3q3 = ahrs->Q.q3 * ahrs->Q.q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * ahrs->Q.q1 - _2q0 * ay - 
                _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * ahrs->Q.q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * ahrs->Q.q3 - _2q1 * ax + 4.0f * q2q2 * ahrs->Q.q3 - _2q2 * ay;
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= ahrs->beta * s0;
		qDot2 -= ahrs->beta * s1;
		qDot3 -= ahrs->beta * s2;
		qDot4 -= ahrs->beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	ahrs->Q.q0 += qDot1 * (1.0f / ahrs->sampleFreq);
	ahrs->Q.q1 += qDot2 * (1.0f / ahrs->sampleFreq);
	ahrs->Q.q2 += qDot3 * (1.0f / ahrs->sampleFreq);
	ahrs->Q.q3 += qDot4 * (1.0f / ahrs->sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(ahrs->Q.q0 * ahrs->Q.q0 + ahrs->Q.q1 * ahrs->Q.q1 + 
            ahrs->Q.q2 * ahrs->Q.q2 + ahrs->Q.q3 * ahrs->Q.q3);
	ahrs->Q.q0 *= recipNorm;
	ahrs->Q.q1 *= recipNorm;
	ahrs->Q.q2 *= recipNorm;
	ahrs->Q.q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

bool Quaternion2Euler(const QUATERNION* q, float *phi, float* theta,
        float* psi)
{
    if (!q)
    {
        return false;
    }
    
    float R_1_1 = 2.0f * q->q0 * q->q0 - 1.0f + 2.0f * q->q1 * q->q1;
    float R_2_1 = 2.0f * (q->q1 * q->q2 - q->q0 * q->q3);
    float R_3_1 = 2.0f * (q->q1 * q->q3 + q->q0 * q->q2);
    float R_3_2 = 2.0f * (q->q2 * q->q3 - q->q0 * q->q1);
    float R_3_3 = 2.0f * q->q0 * q->q0 - 1.0f + 2.0f * q->q3 * q->q3;

    if (phi)
    {
        *phi = atan2(R_3_2, R_3_3);
    }
    
    if (theta)
    {
        *theta = -atan(R_3_1 / sqrt(1.0f - R_3_1 * R_3_1));
    }
    
    if (psi)
    {
        *psi = atan2(R_2_1, R_1_1);
    }

    return true;
}

bool InitMahonyAHRS(float sampleRate, MAHONY_AHRS_INFO* ahrs)
{
    if (!ahrs)
    {
        return false;
    }
    
    ahrs->sampleFreq = sampleRate;
    ahrs->twoKi = twoKiDef;
    ahrs->twoKp = twoKpDef;
    ahrs->integralFBx = 0.0f;
    ahrs->integralFBy = 0.0f;
    ahrs->integralFBz = 0.0f;

    ahrs->Q.q0 = 1.0f;
    ahrs->Q.q1 = 0.0f;
    ahrs->Q.q2 = 0.0f;
    ahrs->Q.q3 = 0.0f;
    
    return true;
}

void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, 
        float az, float mx, float my, float mz, MAHONY_AHRS_INFO* ahrs) 
{
	float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
    
    if (!ahrs)
    {
        return;
    }

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) 
    {
		MahonyAHRSupdateIMU(gx, gy, gz, ax, ay, az, ahrs);
		return;
	}

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;     

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;   

        // Auxiliary variables to avoid repeated arithmetic
        q0q0 = ahrs->Q.q0 * ahrs->Q.q0;
        q0q1 = ahrs->Q.q0 * ahrs->Q.q1;
        q0q2 = ahrs->Q.q0 * ahrs->Q.q2;
        q0q3 = ahrs->Q.q0 * ahrs->Q.q3;
        q1q1 = ahrs->Q.q1 * ahrs->Q.q1;
        q1q2 = ahrs->Q.q1 * ahrs->Q.q2;
        q1q3 = ahrs->Q.q1 * ahrs->Q.q3;
        q2q2 = ahrs->Q.q2 * ahrs->Q.q2;
        q2q3 = ahrs->Q.q2 * ahrs->Q.q3;
        q3q3 = ahrs->Q.q3 * ahrs->Q.q3;   

        // Reference direction of Earth's magnetic field
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
        bx = sqrt(hx * hx + hy * hy);
        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

		// Estimated direction of gravity and magnetic field
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);  
	
		// Error is sum of cross product between estimated direction and measured direction of field vectors
		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		// Compute and apply integral feedback if enabled
		if(ahrs->twoKi > 0.0f) 
        {
			ahrs->integralFBx += ahrs->twoKi * halfex * (1.0f / ahrs->sampleFreq);	// integral error scaled by Ki
			ahrs->integralFBy += ahrs->twoKi * halfey * (1.0f / ahrs->sampleFreq);
			ahrs->integralFBz += ahrs->twoKi * halfez * (1.0f / ahrs->sampleFreq);
			gx += ahrs->integralFBx;	// apply integral feedback
			gy += ahrs->integralFBy;
			gz += ahrs->integralFBz;
		}
		else {
			ahrs->integralFBx = 0.0f;	// prevent integral windup
			ahrs->integralFBy = 0.0f;
			ahrs->integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += ahrs->twoKp * halfex;
		gy += ahrs->twoKp * halfey;
		gz += ahrs->twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / ahrs->sampleFreq));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / ahrs->sampleFreq));
	gz *= (0.5f * (1.0f / ahrs->sampleFreq));
	qa = ahrs->Q.q0;
	qb = ahrs->Q.q1;
	qc = ahrs->Q.q2;
	ahrs->Q.q0 += (-qb * gx - qc * gy - ahrs->Q.q3 * gz);
	ahrs->Q.q1 += (qa * gx + qc * gz - ahrs->Q.q3 * gy);
	ahrs->Q.q2 += (qa * gy - qb * gz + ahrs->Q.q3 * gx);
	ahrs->Q.q3 += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
	recipNorm = invSqrt(ahrs->Q.q0 * ahrs->Q.q0 + ahrs->Q.q1 * ahrs->Q.q1 + 
            ahrs->Q.q2 * ahrs->Q.q2 + ahrs->Q.q3 * ahrs->Q.q3);
	ahrs->Q.q0 *= recipNorm;
	ahrs->Q.q1 *= recipNorm;
	ahrs->Q.q2 *= recipNorm;
	ahrs->Q.q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, 
        float az, MAHONY_AHRS_INFO* ahrs) 
{
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	if (!ahrs)
    {
        return;
    }
    
    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) 
    {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;        

		// Estimated direction of gravity and vector perpendicular to magnetic flux
		halfvx = ahrs->Q.q1 * ahrs->Q.q3 - ahrs->Q.q0 * ahrs->Q.q2;
		halfvy = ahrs->Q.q0 * ahrs->Q.q1 + ahrs->Q.q2 * ahrs->Q.q3;
		halfvz = ahrs->Q.q0 * ahrs->Q.q0 - 0.5f + ahrs->Q.q3 * ahrs->Q.q3;
	
		// Error is sum of cross product between estimated and measured direction of gravity
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		if(ahrs->twoKi > 0.0f) {
			ahrs->integralFBx += ahrs->twoKi * halfex * (1.0f / ahrs->sampleFreq);	// integral error scaled by Ki
			ahrs->integralFBy += ahrs->twoKi * halfey * (1.0f / ahrs->sampleFreq);
			ahrs->integralFBz += ahrs->twoKi * halfez * (1.0f / ahrs->sampleFreq);
			gx += ahrs->integralFBx;	// apply integral feedback
			gy += ahrs->integralFBy;
			gz += ahrs->integralFBz;
		}
		else {
			ahrs->integralFBx = 0.0f;	// prevent integral windup
			ahrs->integralFBy = 0.0f;
			ahrs->integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += ahrs->twoKp * halfex;
		gy += ahrs->twoKp * halfey;
		gz += ahrs->twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / ahrs->sampleFreq));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / ahrs->sampleFreq));
	gz *= (0.5f * (1.0f / ahrs->sampleFreq));
	qa = ahrs->Q.q0;
	qb = ahrs->Q.q1;
	qc = ahrs->Q.q2;
	ahrs->Q.q0 += (-qb * gx - qc * gy - ahrs->Q.q3 * gz);
	ahrs->Q.q1 += (qa * gx + qc * gz - ahrs->Q.q3 * gy);
	ahrs->Q.q2 += (qa * gy - qb * gz + ahrs->Q.q3 * gx);
	ahrs->Q.q3 += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
	recipNorm = invSqrt(ahrs->Q.q0 * ahrs->Q.q0 + ahrs->Q.q1 * ahrs->Q.q1 + 
            ahrs->Q.q2 * ahrs->Q.q2 + ahrs->Q.q3 * ahrs->Q.q3);
	ahrs->Q.q0 *= recipNorm;
	ahrs->Q.q1 *= recipNorm;
	ahrs->Q.q2 *= recipNorm;
	ahrs->Q.q3 *= recipNorm;
}

bool QuaternionConj(const QUATERNION* q_in, QUATERNION* q_out)
{
    if (!q_in || !q_out)
    {
        return false;
    }
    
    q_out->q0 = q_in->q0;
    q_out->q1 = -q_in->q1;
    q_out->q2 = -q_in->q2;
    q_out->q3 = -q_in->q3;

    return true;
}

bool QuaternionProd(const QUATERNION* q_a, const QUATERNION* q_b,
        QUATERNION* q_out)
{
    if (!q_a || !q_b || !q_out)
    {
        return false;
    }
    
    q_out->q0 = q_a->q0 * q_b->q0 - q_a->q1 * q_b->q1 - q_a->q2 * q_b->q2 -
            q_a->q3 * q_b->q3;
    q_out->q1 = q_a->q0 * q_b->q1 + q_a->q1 * q_b->q0 + q_a->q2 * q_b->q3 -
            q_a->q3 * q_b->q2;
    q_out->q2 = q_a->q0 * q_b->q2 - q_a->q1 * q_b->q3 + q_a->q2 * q_b->q0 +
            q_a->q3 * q_b->q1;
    q_out->q3 = q_a->q0 * q_b->q3 + q_a->q1 * q_b->q2 - q_a->q2 * q_b->q1 +
            q_a->q3 * q_b->q0;
    
    return true;
}

bool QuaternionRotate(const COORD_3D* coord_in, const QUATERNION* q,
        COORD_3D* coord_out)
{
    if (!coord_in || !q || !coord_out)
    {
        return false;
    }

    QUATERNION qCoord, qProd, qConj, OXYZ;
    
    qCoord.q0 = 0.0f;
    qCoord.q1 = coord_in->x;
    qCoord.q2 = coord_in->y;
    qCoord.q3 = coord_in->z;
    
    QuaternionProd(q, &qCoord, &qProd);
    QuaternionConj(q, &qConj);
    QuaternionProd(&qProd, &qConj, &OXYZ);
    
    coord_out->x = OXYZ.q1;
    coord_out->y = OXYZ.q2;
    coord_out->z = OXYZ.q3;
    
    return true;
}

bool Euler2Quaternion(float phi, float theta, float psi,
        QUATERNION* q)
{
    if (!q)
    {
        return false;
    }
    
    float c1 = cos(phi / 2);
    float s1 = sin(phi / 2);
    float c2 = cos(theta / 2);
    float s2 = sin(theta / 2);
    float c3 = cos(psi / 2);
    float s3 = sin(psi / 2);
    float c1c2 = c1 * c2;
    float s1s2 = s1 * s2;
    
    q->q0 = c1c2 * c3 - s1s2 * s3;
  	q->q1 = c1c2 * s3 + s1s2 * c3;
	q->q2 = s1 * c2 * c3 + c1 * s2 * s3;
	q->q3 = c1 * s2 * c3 - s1 * c2 * s3;
    
    return true;
}

//====================================================================================================
// END OF CODE
//====================================================================================================
