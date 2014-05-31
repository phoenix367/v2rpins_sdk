/*
 * File:   VfunctionsTest.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 23.05.2014, 22:54:04
 */

#include "VfunctionsTest.hpp"
#include "mono_slam/mono_slam.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(VfunctionsTest);

VfunctionsTest::VfunctionsTest() {
}

VfunctionsTest::~VfunctionsTest() {
}

void VfunctionsTest::setUp() {
}

void VfunctionsTest::tearDown() {
}

void VfunctionsTest::testNormJ() 
{
    mslam::RealType data[4] =
    {
         1.000000000000000000e+00,
         4.125607846284472073e-03,
         3.812299856873190788e-03,
        -1.630117227660913225e-04
    };
    
    mslam::RealType targetData[4 * 4] =
    {
         3.157934715637507523e-05,
        -4.125412418737937729e-03,
        -3.812119270536476565e-03,
         1.630040009994514845e-04,
        -4.125412418737937729e-03,
         9.999671901185915868e-01,
        -1.572730917349752375e-05,
         6.724905854990989835e-07,
        -3.812119270536476565e-03,
        -1.572730917349752375e-05,
         9.999696770106860644e-01,
         6.214201296799663458e-07,
         1.630040009994514845e-04,
         6.724905854990989835e-07,
         6.214201296799663458e-07,
         9.999841833808724711e-01
    };
    
    mslam::RealMatrix normMat(4, 4, targetData);

    mslam::RealMatrix r;
    CPPUNIT_ASSERT_NO_THROW(r = mslam::normJac(mslam::RealVector(data, 
            4)));
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(cv::abs(r - normMat) > 1e-10));
}

void VfunctionsTest::testNormJFailed() 
{
    CPPUNIT_ASSERT_THROW(mslam::normJac(mslam::RealVector()),
            mslam::IncorrectParamException);
}
