/* 
 * File:   VoltageReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 7, 2014, 10:19 PM
 */

#ifndef VOLTAGEREADER_HPP
#define	VOLTAGEREADER_HPP

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <zmq.hpp>

#include "pincontroller/adc_reader.hpp"

class VoltageReader 
{
public:
    VoltageReader();
    virtual ~VoltageReader();
    
private:
    void run();
    
private:
    boost::atomic<bool> stopVariable;
    pc::ADCReader adcReader;
    std::unique_ptr<boost::thread> sensorsThread;
};

#endif	/* VOLTAGEREADER_HPP */
