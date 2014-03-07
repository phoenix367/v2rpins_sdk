/*
 * File:   PwmIndexTest.hpp
 * Author: Ivan Gubochkin
 *
 * Created on Mar 7, 2014, 10:42:28 PM
 */

#ifndef PWMINDEXTEST_HPP
#define	PWMINDEXTEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class PwmIndexTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(PwmIndexTest);

    CPPUNIT_TEST(testPwmIndex);
    CPPUNIT_TEST(testPwmIndexInvalid);

    CPPUNIT_TEST_SUITE_END();

public:
    PwmIndexTest();
    virtual ~PwmIndexTest();
    void setUp();
    void tearDown();

private:
    void testPwmIndex();
    void testPwmIndexInvalid();
};

#endif	/* PWMINDEXTEST_HPP */

