/* 
 * File:   SensorReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 14, 2014, 12:31 AM
 */

#include "SensorReader.hpp"
#include "SensorsController.hpp"

SensorReader::SensorReader() 
: stopVariable(false)
{
}

SensorReader::~SensorReader() 
{
    stopThread();
}

void SensorReader::initThread()
{
    stopVariable = false;    
    readerThread = std::unique_ptr<std::thread>(
                    new std::thread(
                    std::bind(&SensorReader::run, this)));
}

void SensorReader::stopThread()
{
    if (!stopVariable)
    {
        stopVariable = true;
        if (readerThread)
        {
            readerThread->join();
        }
    }
}

void SensorReader::sendData(const std::vector<int8_t>& data)
{
    auto instance = SensorsController::getInstance();
    
    if (instance)
    {
        instance->putMessage(data);
    }
}
