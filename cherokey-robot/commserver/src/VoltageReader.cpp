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
#include "decls.hpp"

namespace cs = cherokey::sensors;

VoltageReader::VoltageReader()
{
    auto instance = ConfigManager::getInstance();
    
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Configuration manager "
                "instance is null.");
    }
    
    auto adcInfo = instance->getAdcInfo();
    
    voltageChannel = adcInfo.voltageChannel;
    currentChannel = adcInfo.currentChannel;
    sensorInfo = instance->getVoltageSensorInfo();
    
    initThread();
}

VoltageReader::~VoltageReader() 
{
}

void VoltageReader::run()
{
    cs::SensorData sensorMessage;

    sensorMessage.set_sensor_id((int) SensorIds::Voltage_sensor);
    sensorMessage.set_sensor_desc(VOLTAGE_SENSOR_NAME);
    auto values = sensorMessage.mutable_sensor_values();
    auto voltageData = values->Add();
    auto currentData = values->Add();

    voltageData->set_associated_name(VOLTAGE_PARAM_VOLTAGE);
    voltageData->set_data_type(cs::REAL);
    currentData->set_associated_name(VOLTAGE_PARAM_CURRENT);
    currentData->set_data_type(cs::REAL);

    float prevVoltageData = 0, prevCurrentData = 0;

    auto delayTime = std::chrono::milliseconds(sensorInfo.measurementRate);
    auto t = std::chrono::high_resolution_clock::now();

    while (!stopVariable)
    {
        pc::ADCReader::ADCValue adcValue;
        adcReader.read(adcValue);

        float voltage = adcValue.adcVoltages[voltageChannel] * 
            sensorInfo.voltageScale;
        voltage = (voltage + prevVoltageData) / 2;
        prevVoltageData = voltage;

        float current = (adcValue.adcVoltages[currentChannel] - 
            sensorInfo.currentOffset) / sensorInfo.currentScale;
        current = (current + prevCurrentData) / 2;
        prevCurrentData = current;

        voltageData->set_real_value(voltage);        
        currentData->set_real_value(current);

        int messageSize = sensorMessage.ByteSize();
        std::vector<int8_t> outArray(messageSize);
        sensorMessage.SerializeToArray(&outArray[0], messageSize);

        sendData(outArray);

        std::this_thread::sleep_until(t + delayTime);
        t += delayTime;
    }
}
