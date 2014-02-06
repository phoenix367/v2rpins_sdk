/* 
 * File:   SensorsController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 27, 2013, 3:06 PM
 */

#include "SensorsController.hpp"
#include "Exceptions.hpp"
#include "GPSReader.hpp"
#include "VoltageReader.hpp"
#include "IMUReader.hpp"

#define MAX_QUEUE_LENGTH                100

extern zmq::context_t gContext;

std::unique_ptr<SensorsController> SensorsController::instance;
const char* SensorsController::SENSORS_CONN_POINT = "inproc://sensors";

SensorsController::SensorsController()
: started(false)
, stopVariable(false)
{

}

SensorsController::~SensorsController() 
{
}

SensorsController* SensorsController::getInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<SensorsController>(
                new SensorsController());
    }
    
    return instance.get();
}

void SensorsController::run()
{
    zmq::socket_t publisher (gContext, ZMQ_PUB);
    
    std::ostringstream stream;
    
    stream << "tcp://" << connectionInfo.ipAddress.to_string() << ":" <<
            connectionInfo.port;
    
    publisher.bind(stream.str().c_str());
        
    zmq::socket_t socket(gContext, ZMQ_PULL);
    socket.bind(SENSORS_CONN_POINT);
    
    processSensorMessages(publisher, socket);
    
    publisher.close();
    socket.close();
}

void SensorsController::startPublisher(const ConnectionInfo& info)
{
    if (started)
    {
        return;
    }
    
    started = true;
    
    try
    {
        stopVariable = false;
        connectionInfo = info;
        producerThread = std::unique_ptr<std::thread>(
                new std::thread(
                std::bind(&SensorsController::run, this)));
    }
    catch (std::exception& e)
    {
        started = false;
        COMM_EXCEPTION(LaunchException, e.what());
    }
}

void SensorsController::stopPublisher()
{
    if (!started)
    {
        return;
    }
    
    stopVariable = true;
    producerThread->join();
    started = false;
}

void SensorsController::processSensorMessages(zmq::socket_t& pubSocket,
        zmq::socket_t& sensorSocket)
{
    GPSReader gpsReader;
    VoltageReader voltageReader;
    IMUReader imuReader;

    while (!stopVariable)
    {
        zmq::pollitem_t items [] = { { sensorSocket, 0, ZMQ_POLLIN, 0 } };
        zmq::poll(items, 1, 100);

        if (items [0].revents & ZMQ_POLLIN) 
        {
            zmq::message_t msg;
            if (sensorSocket.recv(&msg))
            {
                pubSocket.send(msg);
            }
        }
    }
}

void SensorsController::putMessage(const std::vector<int8_t>& msg)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    
    if (messageQueue.size() < MAX_QUEUE_LENGTH)
    {
        messageQueue.push(msg);
    }
}

bool SensorsController::getMessage(std::vector<int8_t>& msg)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    
    if (!messageQueue.empty())
    {
        msg = messageQueue.back();
        messageQueue.pop();
        
        return true;
    }
    
    return false;
}
