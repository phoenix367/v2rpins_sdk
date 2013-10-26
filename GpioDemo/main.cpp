/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on October 13, 2013, 1:20 AM
 */

#include <iostream>
#include <unistd.h>
#include "pincontroller/pincontroller.hpp"

using namespace std;

#define PWM_FREQUENCY 400 // PWM frequency is 10 kHz
#define PWM_PERIOD (1000000 / PWM_FREQUENCY)  // PWM period in microseconds

#define DRIVE_LOW_SPEED (PWM_PERIOD / 2) // 50% of max drive power
#define DRIVE_HIGH_SPEED PWM_PERIOD // 100% of max drive power

/*
 * 
 */
int main(int argc, char** argv) 
{
    try
    {
        pc::GPIOPin pin0(pc::GPIO_PIN::gpio85, 
                pc::GPIO_DIRECTION::output);
        pc::GPIOPin pin1(pc::GPIO_PIN::gpio86, 
                pc::GPIO_DIRECTION::output);
        
        pc::PWM pwm0(pc::PWM_CHANNEL::PWM_0);
        pc::PWM pwm1(pc::PWM_CHANNEL::PWM_1);
        
        pwm0.setPulseParams(DRIVE_LOW_SPEED, PWM_PERIOD);
        pwm0.init();
        pwm1.setPulseParams(DRIVE_LOW_SPEED, PWM_PERIOD);
        pwm1.init();
        sleep(5);
        
        pin0.setLogicalLevel(pc::GPIO_LOGIC_LEVEL::high);
        
        pwm0.setPulseParams(DRIVE_HIGH_SPEED, PWM_PERIOD);
        pwm0.init();
        pwm1.setPulseParams(DRIVE_HIGH_SPEED, PWM_PERIOD);
        pwm1.init();
        sleep(5);
    }
    catch (pc::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

