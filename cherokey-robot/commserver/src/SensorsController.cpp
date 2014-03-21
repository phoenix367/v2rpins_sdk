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
#include "PIDController.hpp"

#define MAX_QUEUE_LENGTH                100

extern zmq::context_t gContext;

std::unique_ptr<SensorsController> SensorsController::instance;

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
    processSensorMessages(publisher);    
    publisher.close();
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

void SensorsController::processSensorMessages(zmq::socket_t& pubSocket)
{
    auto pidInstance = PIDController::getInstance();
    
    if (!pidInstance)
    {
        COMM_EXCEPTION(NullPointerException, "PID controller instance "
                "is null");
    }
    
    GPSReader gpsReader;
    VoltageReader voltageReader;
    IMUReader imuReader;
    
    pidInstance->setIMUReader(&imuReader);

    while (!stopVariable)
    {
        std::vector<int8_t> msg;
        
        if (getMessage(msg))
        {
            pubSocket.send(&msg[0], msg.size());
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
