/*
 * File:   PwmIndexTest.cpp
 * Author: Ivan Gubochkin
 *
 * Created on Mar 7, 2014, 10:42:29 PM
 */

#include "PwmIndexTest.hpp"
#include "pincontroller/global.hpp"
#include "pincontroller/exceptions.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(PwmIndexTest);

PwmIndexTest::PwmIndexTest() 
{
}

PwmIndexTest::~PwmIndexTest() 
{
}

void PwmIndexTest::setUp() 
{
}

void PwmIndexTest::tearDown() 
{
}

void PwmIndexTest::testPwmIndex() 
{
    pc::PWM_CHANNEL pwm = pc::pwmFromIndex(0);
    
    CPPUNIT_ASSERT(pwm == pc::PWM_CHANNEL::PWM_0);
    
    pwm = pc::pwmFromIndex(1);
    CPPUNIT_ASSERT(pwm == pc::PWM_CHANNEL::PWM_1);

    pwm = pc::pwmFromIndex(2);
    CPPUNIT_ASSERT(pwm == pc::PWM_CHANNEL::PWM_2);

    pwm = pc::pwmFromIndex(3);
    CPPUNIT_ASSERT(pwm == pc::PWM_CHANNEL::PWM_3);
}

void PwmIndexTest::testPwmIndexInvalid() 
{
    try
    {
        pc::PWM_CHANNEL pwm = pc::pwmFromIndex(4);
        CPPUNIT_FAIL("No exception threw");
    }
    catch (pc::IncorrectParamException&)
    {
    }
}
