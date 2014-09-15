/*
 * File:   slamFuncTest.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 13.09.2014, 23:23:17
 */

#include "slamFuncTest.hpp"
#include "mono_slam/declares.hpp"
#include "mono_slam/slam_functions.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(slamFuncTest);

slamFuncTest::slamFuncTest() {
}

slamFuncTest::~slamFuncTest() {
}

void slamFuncTest::setUp() {
}

void slamFuncTest::tearDown() {
}

void slamFuncTest::testHuFunc() 
{
    mslam::RealType yiData[3] = {
        -0.54473790876651229,
        -0.0045914696402309966,
        0.84238048833577783
    };
    mslam::RealType uv_uData[2] = {
        34.729803785608539,
        127.80831654298801
    };
    mslam::RealMatrix31 yi(yiData);
    mslam::RealMatrix uv_u(2, 1, uv_uData);
    
    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.f = 2.1735000000000002;
    
    auto res = mslam::hu(yi, cam);
    
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(res - uv_u) > 1e-15) == 0);
}

void slamFuncTest::testDistortFmFunc() 
{
    mslam::RealType uv_uData[2] = {
        34.729803785608539,
        127.80831654298801
    };
    mslam::RealType uvdData[2] = {
        49.262988088995215,
        127.93081338675638
    };
    mslam::RealMatrix uv_u(2, 1, uv_uData);
    mslam::RealMatrix uvd(2, 1, uvdData);

    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.k1 = 0.063329999999999997;
    cam.k2 = 0.013899999999999999;
    
    auto res = mslam::distort_fm(uv_u, cam);

    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(res - uvd) > 1e-13) == 0);
}

void slamFuncTest::testMFunc()
{
    mslam::RealType a = -0.55425841752082594, b = 0.0091594900973087975;
    mslam::RealType mData[3] = {
        -0.52629080640410209,
        -0.009159362023354033,
        0.85025531058726422
    };
    cv::Matx<mslam::RealType, 3, 1> m(mData);
    
    auto res = mslam::m(a, b);
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(cv::Mat(res - m)) > 1e-15) == 0);
}

void slamFuncTest::testHighInverseDepthFunc()
{
    
}
