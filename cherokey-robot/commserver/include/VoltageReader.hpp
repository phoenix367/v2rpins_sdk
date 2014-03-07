/* 
 * File:   VoltageReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 7, 2014, 10:19 PM
 */

#ifndef VOLTAGEREADER_HPP
#define	VOLTAGEREADER_HPP

#include "pincontroller/adc_reader.hpp"
#include "SensorReader.hpp"

class VoltageReader : public SensorReader
{
public:
    VoltageReader();
    virtual ~VoltageReader();
    
private:
    virtual void run();
    
private:
    pc::ADCReader adcReader;
    int voltageChannel;
    int currentChannel;
};

#endif	/* VOLTAGEREADER_HPP */
