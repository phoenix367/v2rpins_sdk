/* 
 * File:   VoltageReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 7, 2014, 10:19 PM
 */

#include "VoltageReader.hpp"
#include "Exceptions.hpp"
#include "sensors.pb.h"
#include "GPSReader.hpp"
#include "SensorsController.hpp"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace cs = cherokey::sensors;

VoltageReader::VoltageReader()
{
    initThread();
}

VoltageReader::~VoltageReader() 
{
}

void VoltageReader::run()
{
    boost::asio::io_service io;
    auto delayTime = boost::posix_time::milliseconds(250);
    boost::asio::deadline_timer t(io, delayTime);

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

    float prevVoltageData = 0, prevCurrentData = 0;

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

        sendData(outArray);

        t.wait();
        t.expires_at(t.expires_at() + delayTime);
    }
}
