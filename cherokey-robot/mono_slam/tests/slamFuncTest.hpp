/*
 * File:   slamFuncTest.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 13.09.2014, 23:23:16
 */

#ifndef SLAMFUNCTEST_HPP
#define	SLAMFUNCTEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class slamFuncTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(slamFuncTest);

    CPPUNIT_TEST(testHuFunc);
    CPPUNIT_TEST(testDistortFmFunc);
    CPPUNIT_TEST(testMFunc);
    CPPUNIT_TEST(testHighInverseDepthFunc);
    CPPUNIT_TEST(testPredCamMeasurements);
    CPPUNIT_TEST(testFindRANSACFeatures);
    CPPUNIT_TEST(testfvFunction);
    CPPUNIT_TEST(testdq3_by_dq1Function);
    CPPUNIT_TEST(testdqomegadt_by_domegaFunction);
    CPPUNIT_TEST(testfunc_QFunction);
    CPPUNIT_TEST(testdq3_by_dq2Function);
    CPPUNIT_TEST(testdfv_by_dxvFunction);
    CPPUNIT_TEST(testjacob_undistor_fmFunction);
    CPPUNIT_TEST(testundistor_fmFunction);
    CPPUNIT_TEST(testhinvFunction);
    CPPUNIT_TEST(testdhu_dhrlFunction);
    CPPUNIT_TEST(testdhd_dhuFunction);
    CPPUNIT_TEST(testdh_dhrlFunction);
    CPPUNIT_TEST(testdR_by_dqzFunction);
    CPPUNIT_TEST(testdR_by_dqyFunction);
    CPPUNIT_TEST(testdR_by_dqxFunction);
    CPPUNIT_TEST(testdR_by_dq0Function);
    CPPUNIT_TEST(testdRq_times_a_by_dqFunction);
    CPPUNIT_TEST(testdhrl_dqwrFunction);
    CPPUNIT_TEST(testdh_dqwrFunction);
    CPPUNIT_TEST(testdhrl_drwFunction);
    CPPUNIT_TEST(testdh_drwFunction);
    CPPUNIT_TEST(testdh_dxvFunction);
    CPPUNIT_TEST(testdhrl_dyFunction);
    CPPUNIT_TEST(testdh_dyFunction);

    CPPUNIT_TEST_SUITE_END();

public:
    slamFuncTest();
    virtual ~slamFuncTest();
    void setUp();
    void tearDown();

private:
    void testHuFunc();
    void testDistortFmFunc();
    void testMFunc();
    void testHighInverseDepthFunc();
    void testPredCamMeasurements();
    void testFindRANSACFeatures();
    void testfvFunction();
    void testdq3_by_dq1Function();
    void testdqomegadt_by_domegaFunction();
    void testfunc_QFunction();
    void testdq3_by_dq2Function();
    void testdfv_by_dxvFunction();
    void testjacob_undistor_fmFunction();
    void testundistor_fmFunction();
    void testhinvFunction();
    void testdhu_dhrlFunction();
    void testdhd_dhuFunction();
    void testdh_dhrlFunction();
    void testdR_by_dqzFunction();
    void testdR_by_dqyFunction();
    void testdR_by_dqxFunction();
    void testdR_by_dq0Function();
    void testdRq_times_a_by_dqFunction();
    void testdhrl_dqwrFunction();
    void testdh_dqwrFunction();
    void testdhrl_drwFunction();
    void testdh_drwFunction();
    void testdh_dxvFunction();
    void testdhrl_dyFunction();
    void testdh_dyFunction();
};

#endif	/* SLAMFUNCTEST_HPP */

