/*
 * File:   QuaternionOperations.hpp
 * Author: Ivan Gubochkin
 *
 * Created on Feb 16, 2014, 12:14:56 AM
 */

#ifndef QUATERNIONOPERATIONS_HPP
#define	QUATERNIONOPERATIONS_HPP

#include <cppunit/extensions/HelperMacros.h>

class QuaternionOperations : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(QuaternionOperations);

    CPPUNIT_TEST(testQuaternionConj);
    CPPUNIT_TEST(testQuaternionProd);
    CPPUNIT_TEST(testQuaternionRotate);

    CPPUNIT_TEST_SUITE_END();

public:
    QuaternionOperations();
    virtual ~QuaternionOperations();
    void setUp();
    void tearDown();

private:
    void testQuaternionConj();
    void testQuaternionProd();
    void testQuaternionRotate();
};

#endif	/* QUATERNIONOPERATIONS_HPP */

