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
    AHRS_INFO ahrs;
    ahrs.q0 = 9.999995e-01f;
    ahrs.q1 = -9.697057e-04f;
    ahrs.q2 = 2.426228e-04f;
    ahrs.q3 = 8.726642e-08f;
    
    float phi, theta, psi;
    CPPUNIT_ASSERT(Quaternion2Euler(&ahrs, &phi, &theta, &psi));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(phi * 180 / M_PI, -1.111201e-01f, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(theta * 180 / M_PI, 2.780252e-02f, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(psi * 180 / M_PI, -1.696028e-05f, 1e-5f);
}

void TestEuler::testNullPtr()
{
    CPPUNIT_ASSERT_ASSERTION_FAIL(CPPUNIT_ASSERT(
            Quaternion2Euler(NULL, NULL, NULL, NULL)));
}
