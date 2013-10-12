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

/*
 * 
 */
int main(int argc, char** argv) 
{
    try
    {
        pc::PWM pwm(pc::PWM_CHANNEL::PWM_0);
        
        pwm.setPulseParams(1, 2);
        pwm.init();
        sleep(5);
    }
    catch (pc::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

