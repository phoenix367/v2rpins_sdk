/* 
 * File:   SensorsController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 27, 2013, 3:06 PM
 */

#include "SensorsController.hpp"
#include "Exceptions.hpp"
#include "sensors.pb.h"
#include "GPSReader.hpp"

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <zmq.hpp>

namespace cs = cherokey::sensors;

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
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(250));
    
    zmq::socket_t publisher (gContext, ZMQ_PUB);
    
    std::ostringstream stream;
    
    stream << "tcp://" << connectionInfo.ipAddress.to_string() << ":" <<
            connectionInfo.port;
    
    publisher.bind(stream.str().c_str());
    
    float prevVoltageData = 0, prevCurrentData = 0;
    cs::SensorData sensorMessage;

    sensorMessage.set_sensor_id(0);
    sensorMessage.set_sensor_desc("voltage_current");
    auto values = sensorMessage.mutable_sensor_values();
    auto voltageData = values->Add();
    auto currentData = values->Add();
    
    voltageData->set_associated_name("voltage");
    voltageData->set_data_type(cs::REAL);
    currentData->set_associated_name("current");
    currentData->set_data_type(cs::REAL);
    
    GPSReader gpsReader;

    while (!stopVariable)
    {
        pc::ADCReader::ADCValue adcValue;
        adcReader.read(adcValue);
        
        float voltage = adcValue.adcVoltages[0] * 3.593;
        voltage = (voltage + prevVoltageData) / 2;
        prevVoltageData = voltage;
        
        float current = -(adcValue.adcVoltages[1] - 2) / (2 * 0.185);
        current = (current + prevCurrentData) / 2;
        prevCurrentData = current;
                
        voltageData->set_real_value(voltage);        
        currentData->set_real_value(current);
        
        int messageSize = sensorMessage.ByteSize();
        std::vector<int8_t> outArray(messageSize);
        sensorMessage.SerializeToArray(&outArray[0], messageSize);
        
        publisher.send(&outArray[0], messageSize);
        
        t.wait();
        t.expires_at(t.expires_at() + boost::posix_time::milliseconds(250));
    }
    
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
        producerThread = std::unique_ptr<boost::thread>(
                new boost::thread(
                boost::bind(&SensorsController::run, this)));
    }
    catch (boost::exception& e)
    {
        started = false;
        COMM_EXCEPTION(LaunchException, boost::diagnostic_information(e));
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
