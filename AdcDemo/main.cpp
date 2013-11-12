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

    // Читаем значение напряжения на входах АЦП
    pc::ADCReader::ADCValue adcValue;
    adcReader.read(adcValue);
    
    static std::string eol = "\x1B[K\n";

    std::cout << "\x1B[0;0fVirt2real ADC state..." << eol << eol;
    
    // Выводим прочитанные значения в удобном для восприятия
    // формате.
    for (int i = 0; i < pc::ADCReader::ADC_COUNT; i++)
    {
        std::cout << "ADC channel " << i << ". raw value=" <<
                std::setw(4) << adcValue.adcValuesRaw[i] << 
                "; voltage=" << std::setw(5) << std::setprecision(4) <<
                adcValue.adcVoltages[i] << " volts" << eol;
    }
    
    std::cout << eol << "Press Ctrl+C to exit." << eol << "\x1B[J";
}

/*
 * Данная программа демонстрирует как можно читать данные
 * из АЦП платы Virt2real.
 */
int main(int argc, char** argv) 
{
    // Создаем объект типа ADCReader
    pc::ADCReader adcReader;
    
    std::system("clear");
    // В бесконечном цикле выводим на экран состояние АЦП платы
    while (true)
    {   
        printfADCState(adcReader);
        usleep(250000);
    }
    
    return 0;
}
