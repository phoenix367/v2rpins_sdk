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
};

#endif	/* SLAMFUNCTEST_HPP */

