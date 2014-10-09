/*
 * File:   slamFuncTest.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 13.09.2014, 23:23:17
 */

#include "slamFuncTest.hpp"
#include "mono_slam/declares.hpp"
#include "mono_slam/slam_functions.hpp"
#include "load_util.hpp"


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
    
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(res - uv_u) > 1e-14) == 0);
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
    mslam::RealType yinitData[] = {
        0,
        0,
        0,
        -0.348435644970339964,
        -0.121573649901980524,
        1
    };
    mslam::RealMatrix61 yinit(yinitData);
    
    mslam::RealType t_wcData[] = {
        0.0108888137549603657,
        0.00251060415912785894,
        0.00311901665777174642,
    };
    mslam::RealMatrix31 t_wc(t_wcData);
    
    mslam::RealType r_wcData[] = {
         0.999970880513577187,    0.000357468264786742608,  0.00762301392765437052,
        -0.000294558034737228442, 0.999965906650850833,    -0.00825219798033385854,
        -0.00762570393247167123,  0.00824971226056550599,   0.999936893452358255
    };
    mslam::RealMatrix33 r_wc(r_wcData);

    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.k1 = 0.063329999999999997;
    cam.k2 = 0.013899999999999999;
    cam.f = 2.1735000000000002;
    cam.nRows = 240;
    cam.nCols = 320;
    
    mslam::RealType targetData[] = {
        89.1484448208473736,
        154.018333421641529
    };
    mslam::RealMatrix m(2, 1, targetData);
    
    auto res = mslam::hi_inverse_depth(yinit, t_wc, r_wc, cam);
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(cv::Mat(res - m)) > 1e-13) == 0);
}

void slamFuncTest::testPredCamMeasurements()
{
    auto x_k_k = test::loadMatrix("tests/data/test_pred_cam/x_k_k.dat");
    mslam::RealVector x_k_k_v(x_k_k, true);
    static const int FEATURES_COUNT = 12;
    
    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.k1 = 0.063329999999999997;
    cam.k2 = 0.013899999999999999;
    cam.f = 2.1735000000000002;
    cam.nRows = 240;
    cam.nCols = 320;

    std::vector<mslam::FeatureInfo> featuresInfo(FEATURES_COUNT);
    for (size_t i = 0; i < FEATURES_COUNT; i++)
    {
        featuresInfo[i].type = mslam::inversedepth;
    }
    
    mslam::predict_camera_measurements(x_k_k_v, cam, featuresInfo);
    
    CPPUNIT_ASSERT(featuresInfo.size() == FEATURES_COUNT);
    for (size_t i = 0; i < FEATURES_COUNT; i++)
    {
        std::ostringstream stream;
        stream << "tests/data/test_pred_cam/h_" << i << ".dat";
        auto h_t = test::loadMatrix(stream.str());
        
        CPPUNIT_ASSERT(cv::countNonZero(cv::abs(h_t.t() - featuresInfo[i].h) > 
                1e-13) == 0);
    }
}
