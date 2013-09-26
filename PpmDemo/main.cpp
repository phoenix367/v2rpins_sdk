/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 20:42
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
        pc::PWM pwm(pc::PWM_0);
        
        while (true)
        {
            pwm.setPulseParams(2200, 20000);
            pwm.init();
            
            sleep(2);

            pwm.setPulseParams(4200, 20000);
            pwm.init();
            
            sleep(2);
        }
    }
    catch (pc::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
