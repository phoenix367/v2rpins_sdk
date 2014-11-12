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
};

#endif	/* SLAMFUNCTEST_HPP */

