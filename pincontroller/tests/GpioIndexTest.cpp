/*
 * File:   GpioIndexTest.cpp
 * Author: Ivan Gubochkin
 *
 * Created on Feb 24, 2014, 11:22:26 PM
 */

#include "GpioIndexTest.hpp"
#include "pincontroller/global.hpp"
#include "pincontroller/exceptions.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(GpioIndexTest);

GpioIndexTest::GpioIndexTest() {
}

GpioIndexTest::~GpioIndexTest() {
}

void GpioIndexTest::setUp() {
}

void GpioIndexTest::tearDown() {
}

void GpioIndexTest::testGpioFromIndex() {
    pc::GPIO_PIN pin = pc::gpioPinFromIndex(85);
    
    CPPUNIT_ASSERT(pin == pc::GPIO_PIN::gpio85);
}

void GpioIndexTest::testGpioFromIndexInv() {
    try
    {
        pc::GPIO_PIN pin = pc::gpioPinFromIndex(123);
        CPPUNIT_FAIL("No exception threw");
    }
    catch (pc::IncorrectParamException&)
    {
    }
}
