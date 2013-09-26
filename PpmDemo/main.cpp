/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 20:42
 */

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include "pincontroller/pincontroller.hpp"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    try
    {
        pc::PWM pwm1(pc::PWM_CHANNEL::PWM_0), 
                pwm2(pc::PWM_CHANNEL::PWM_1);
        
        while (true)
        {
            pwm1.setPulseParams(2200, 20000);
            pwm1.init();
            
            pwm2.setPulseParams(2200, 10000);
            pwm2.init();

            sleep(2);

            pwm1.setPulseParams(4200, 20000);
            pwm1.init();
            
            pwm2.setPulseParams(4200, 10000);
            pwm2.init();

            sleep(2);
        }
    }
    catch (pc::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
