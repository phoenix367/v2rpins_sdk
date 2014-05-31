/*
 * File:   VfunctionsTest.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 23.05.2014, 22:54:03
 */

#ifndef VFUNCTIONSTEST_HPP
#define	VFUNCTIONSTEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class VfunctionsTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VfunctionsTest);

    CPPUNIT_TEST(testNormJ);
    CPPUNIT_TEST(testNormJFailed);

    CPPUNIT_TEST_SUITE_END();

public:
    VfunctionsTest();
    virtual ~VfunctionsTest();
    void setUp();
    void tearDown();

private:
    void testNormJ();
    void testNormJFailed();
};

#endif	/* VFUNCTIONSTEST_HPP */