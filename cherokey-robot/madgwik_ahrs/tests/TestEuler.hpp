/*
 * File:   TestEuler.hpp
 * Author: Ivan Gubochkin
 *
 * Created on Feb 8, 2014, 3:32:33 PM
 */

#ifndef TESTEULER_HPP
#define	TESTEULER_HPP

#include <cppunit/extensions/HelperMacros.h>

class TestEuler : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestEuler);

    CPPUNIT_TEST(testQuaternion2Euler);
    CPPUNIT_TEST(testNullPtr);

    CPPUNIT_TEST_SUITE_END();

public:
    TestEuler();
    virtual ~TestEuler();
    void setUp();
    void tearDown();

private:
    void testQuaternion2Euler();
    void testNullPtr();
};

#endif	/* TESTEULER_HPP */

