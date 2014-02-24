/*
 * File:   GpioIndexTest.hpp
 * Author: Ivan Gubochkin
 *
 * Created on Feb 24, 2014, 11:22:25 PM
 */

#ifndef GPIOINDEXTEST_HPP
#define	GPIOINDEXTEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class GpioIndexTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(GpioIndexTest);

    CPPUNIT_TEST(testGpioFromIndex);
    CPPUNIT_TEST(testGpioFromIndexInv);

    CPPUNIT_TEST_SUITE_END();

public:
    GpioIndexTest();
    virtual ~GpioIndexTest();
    void setUp();
    void tearDown();

private:
    void testGpioFromIndex();
    void testGpioFromIndexInv();
};

#endif	/* GPIOINDEXTEST_HPP */

