/*
 * File:   TestEuler.cpp
 * Author: Ivan Gubochkin
 *
 * Created on Feb 8, 2014, 3:32:35 PM
 */

#include "TestEuler.hpp"
#include "madgwik_ahrs.h"

#include <cmath>

CPPUNIT_TEST_SUITE_REGISTRATION(TestEuler);

TestEuler::TestEuler() 
{
}

TestEuler::~TestEuler() 
{
}

void TestEuler::setUp() 
{
}

void TestEuler::tearDown() 
{
}

void TestEuler::testQuaternion2Euler() 
{
    QUATERNION q;
    q.q0 = 9.999995e-01f;
    q.q1 = 9.697057e-04f;
    q.q2 = -2.426228e-04f;
    q.q3 = -8.726642e-08f;
    
    float phi, theta, psi;
    CPPUNIT_ASSERT(Quaternion2Euler(&q, &phi, &theta, &psi));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(phi * 180 / M_PI, -0.11149855984585305f, 
            1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(theta * 180 / M_PI, 2.770847e-02f, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(psi * 180 / M_PI, -1.701804e-05f, 1e-5f);
}

void TestEuler::testQuaternion2EulerNullPtr()
{
    CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT(
            Quaternion2Euler(NULL, NULL, NULL, NULL)));
}

void TestEuler::testEuler2Quaternion()
{
    QUATERNION q, qOut, qNorm;
    q.q0 = 1.0f;
    q.q1 = 0.1f;
    q.q2 = 0.2f;
    q.q3 = 0.3f;
    
    float phi, theta, psi;
    
    QuaternionNormalize(&q, &qNorm);
    CPPUNIT_ASSERT(Quaternion2Euler(&q, &phi, &theta, &psi));
    CPPUNIT_ASSERT(Euler2Quaternion(phi, theta, psi, &qOut));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(qOut.q0, qNorm.q0, 1e-4f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qOut.q1, qNorm.q1, 1e-4f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qOut.q2, qNorm.q2, 1e-4f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qOut.q3, qNorm.q3, 1e-4f);
}
