/*
 * File:   ekfTest.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 29.05.2014, 0:50:23
 */

#ifndef EKFTEST_HPP
#define	EKFTEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class ekfTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ekfTest);

    CPPUNIT_TEST(testUpdate);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    ekfTest();
    virtual ~ekfTest();
    void setUp();
    void tearDown();

private:
    void testUpdate();
    void testFailedMethod();
};

#endif	/* EKFTEST_HPP */
