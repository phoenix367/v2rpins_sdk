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

void VfunctionsTest::testq2rFunction()
{
    mslam::RealType qData[4] = {
        0.999981401328104647,
        0.00372253651322187237,
        0.00480838103813518037,
        0.000468179003859795124
    };
    mslam::RealVector q(qData, 4);
    
    mslam::RealType targetData[] = {
         0.99995332056042463,     -0.000900541844736344279,  0.00962006884414099393,
         0.000972139340672111842,  0.999971847060655894,    -0.00744043219188406449,
        -0.00961309759039462482,   0.00744943692406251426,   0.999926044387399515
    };
    mslam::RealMatrix target(3, 3, targetData);
    
    auto res = mslam::q2r(q);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(cv::abs(res - target) > 1e-15));
}

void VfunctionsTest::testv2qFunction()
{
    mslam::RealType vData[3] = {
        0.00744521313538017843,
        0.00961693688994200505,
        0.000936372857725279459
    };
    mslam::RealVector v(vData, 3, true);
    
    mslam::RealType qData[4] = {
        0.999981400874166759,
        0.00372258348858547916,
        0.00480843863383081417,
        0.000468183526239605982
    };
    mslam::RealMatrix q(4, 1, qData, true);

    auto res = mslam::v2q(v);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(cv::abs(mslam::v2m(res) - q) > 
            1e-15));
}

void VfunctionsTest::testqprodFunction()
{
    mslam::RealType qData[4] = {
        0.999981401328104758,
        0.00372253651322209007,
        0.0048083810381358439,
        0.000468179003860142828
    };
    mslam::RealVector q(qData, 4, true);

    mslam::RealType pData[4] = {
        0.999981400874166759,
        0.00372258348858547916,
        0.00480843863383081417,
        0.000468183526239605982
    };
    mslam::RealVector p(pData, 4, true);

    mslam::RealType qpData[4] = {
        0.999925605096383951,
        0.0074449815255538624,
        0.0096166408148683543,
        0.000936345103314383301
    };
    mslam::RealMatrix qp(4, 1, pData, true);

    auto res = mslam::qprod(p, q);
    //std::cout << mslam::v2m(res) - qp << std::endl;

    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(cv::abs(mslam::v2m(res) - qp) > 
            1e-15));
}
