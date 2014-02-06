/* 
 * File:   SensorReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 14, 2014, 12:31 AM
 */

#include "SensorReader.hpp"
#include "SensorsController.hpp"

extern zmq::context_t gContext;

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
    socket = std::unique_ptr<zmq::socket_t>(new zmq::socket_t(
            gContext, ZMQ_PUSH));
    socket->connect(SensorsController::SENSORS_CONN_POINT);
    
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
        
        if (socket)
        {
            socket->close();
        }
    }
}

void SensorReader::sendData(const std::vector<int8_t>& data)
{
    if (socket)
    {
        socket->send(&data[0], data.size());
    }
}
