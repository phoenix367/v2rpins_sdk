/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on October 1, 2013, 11:11 PM
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
    pc::ADCReader adcReader;
    
    while (true)
    {
        pc::ADCReader::ADCValue adcValue;
        
        adcReader.read(adcValue);
        
        for (int i = 0; i < pc::ADCReader::ADC_COUNT; i++)
        {
            std::cout << adcValue.adcValuesRaw[i] << " ";
        }
        
        std::cout << std::endl;
        sleep(1);
    }
    
    return 0;
}
