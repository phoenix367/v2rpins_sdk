/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on October 1, 2013, 11:11 PM
 */

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include "pincontroller/pincontroller.hpp"

using namespace std;

void printfADCState(pc::ADCReader& adcReader)
{
    std::system("clear");

    pc::ADCReader::ADCValue adcValue;
    adcReader.read(adcValue);

    std::cout << "Virt2real ADC state..." << std::endl << std::endl;
    
    for (int i = 0; i < pc::ADCReader::ADC_COUNT; i++)
    {
        std::cout << "ADC channel " << i << ". raw value=" <<
                std::setw(4) << adcValue.adcValuesRaw[i] << 
                "; voltage=" << std::setw(5) << std::setprecision(4) <<
                adcValue.adcVoltages[i] << " volts" << std::endl;
    }
}

/*
 * 
 */
int main(int argc, char** argv) 
{
    pc::ADCReader adcReader;
    
    while (true)
    {       
        printfADCState(adcReader);
        sleep(1);
    }
    
    return 0;
}
