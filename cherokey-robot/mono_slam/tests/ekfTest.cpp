/*
 * File:   ekfTest.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 29.05.2014, 0:50:24
 */

#include "ekfTest.hpp"
#include "mono_slam/mono_slam.hpp"
#include "load_util.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(ekfTest);

ekfTest::ekfTest() 
{
}

ekfTest::~ekfTest() 
{
}

void ekfTest::setUp() 
{
}

void ekfTest::tearDown() 
{
}

void ekfTest::testUpdate() 
{
    auto H = test::loadMatrix("tests/data/test_update_data/H.dat");
    auto R = test::loadMatrix("tests/data/test_update_data/R.dat");
    auto x_km1_k = test::loadMatrix("tests/data/test_update_data/x_km1_k.dat");
    auto p_km1_k = test::loadMatrix("tests/data/test_update_data/p_km1_k.dat");
    auto h = test::loadMatrix("tests/data/test_update_data/h_v.dat");
    auto z = test::loadMatrix("tests/data/test_update_data/z.dat");
    auto p_k_k = test::loadMatrix("tests/data/test_update_data/p_k_k.dat");
    auto x_k_k = test::loadMatrix("tests/data/test_update_data/x_k_k.dat");
    
    mslam::EKF ekf;
    mslam::RealVector h_v(h, true);
    mslam::RealVector z_v(z, true);
    mslam::RealVector x_km1_k_v(x_km1_k, true);
    
    ekf.update(x_km1_k_v, p_km1_k, H, R, z_v, h_v);
    auto x_res = ekf.getX();
    auto p_res = ekf.getP();
    
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(mslam::v2m(x_res) - x_k_k) > 
            1e-15) == 0);    
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(p_res - p_k_k) > 1e-15) == 0);
}

void ekfTest::testFailedMethod() 
{
    CPPUNIT_ASSERT(false);
}
