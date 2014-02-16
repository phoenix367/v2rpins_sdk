/*
 * File:   QuaternionOperations.cpp
 * Author: Ivan Gubochkin
 *
 * Created on Feb 16, 2014, 12:14:57 AM
 */

#include "QuaternionOperations.hpp"
#include "madgwik_ahrs.h"


CPPUNIT_TEST_SUITE_REGISTRATION(QuaternionOperations);

QuaternionOperations::QuaternionOperations() 
{
}

QuaternionOperations::~QuaternionOperations() 
{
}

void QuaternionOperations::setUp() 
{
}

void QuaternionOperations::tearDown() 
{
}

void QuaternionOperations::testQuaternionConj() 
{
    QUATERNION q, qTarget;
    q.q0 = 9.999995e-01f;
    q.q1 = 9.697057e-04f;
    q.q2 = -2.426228e-04f;
    q.q3 = -8.726642e-08f;

    qTarget.q0 = 9.999995e-01f;
    qTarget.q1 = -9.697057e-04f;
    qTarget.q2 = 2.426228e-04f;
    qTarget.q3 = 8.726642e-08f;

    QUATERNION qConj;
    CPPUNIT_ASSERT(QuaternionConj(&q, &qConj));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q0, qConj.q0, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q1, qConj.q1, 1e-8f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q2, qConj.q2, 1e-8f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q3, qConj.q3, 1e-8f);
}

void QuaternionOperations::testQuaternionProd() 
{
    QUATERNION q_a, q_b, qTarget, qProd;
    
    q_a.q0 = 9.999955e-01f;
    q_a.q1 = -2.908411e-03f;
    q_a.q2 = 7.327030e-04f;
    q_a.q3 = 1.942619e-06f;

    q_b.q0 = 0.000000e+00f;
    q_b.q1 = -1.632653e-02f;
    q_b.q2 = -6.530612e-02f;
    q_b.q3 = 1.004082e+00f;

    qTarget.q0 = -1.584820e-06f;
    qTarget.q1 = -1.559064e-02f;
    qTarget.q2 = -6.238558e-02f;
    qTarget.q3 = 1.004279e+00f;
    
    CPPUNIT_ASSERT(QuaternionProd(&q_a, &q_b, &qProd));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q0, qProd.q0, 1e-8f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q1, qProd.q1, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q2, qProd.q2, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qTarget.q3, qProd.q3, 1e-5f);
}

void QuaternionOperations::testQuaternionRotate() 
{
    COORD_3D coordIn, coordTarget, coordOut;
    QUATERNION q;
    
    q.q0 = 9.999955e-01f;
    q.q1 = -2.908411e-03f;
    q.q2 = 7.327030e-04;
    q.q3 = 1.942619e-06;
    
    coordTarget.x = -1.485461e-02f;
    coordTarget.y = -5.946447e-02f;
    coordTarget.z = 1.004467e+00f;
    
    coordIn.x = -1.632653e-02f;
    coordIn.y = -6.530612e-02f;
    coordIn.z = 1.004082e+00f;
    
    CPPUNIT_ASSERT(QuaternionRotate(&coordIn, &q, &coordOut));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(coordTarget.x, coordOut.x, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coordTarget.y, coordOut.y, 1e-5f);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coordTarget.z, coordOut.z, 1e-5f);
}
