/* 
 * File:   WiFiInfoParser.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on June 1, 2014, 1:04 AM
 */

#include "WiFiInfoParser.hpp"
#include "decls.hpp"
#include "sensors.pb.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace cs = cherokey::sensors;

const std::string WiFiInfoParser::WIRELESS_INFO_FILE_NAME = 
    "/proc/net/wireless";

WiFiInfoParser::WiFiInfoParser() 
{
    initThread();
}

WiFiInfoParser::~WiFiInfoParser() 
{
}

bool WiFiInfoParser::readInfo()
{
    std::ifstream stream(WIRELESS_INFO_FILE_NAME);
    interfacesData.clear();
    
    if (!stream)
    {
        return false;
    }
    
    int lineCount = 0;
    while (!stream.eof())
    {
        std::string strLine;
        std::getline(stream, strLine);
        
        if (!strLine.empty())
        {
            lineCount++;
            
            if (lineCount > 2)
            {
                std::vector<std::string> splitData;
                
                std::istringstream columnStream(strLine);
                while (!columnStream.eof())
                {
                    std::string item;
                    columnStream >> item;

                    splitData.push_back(item);
                }
                
                if (splitData.size() == 11)
                {
                    WiFiSignalInfo wifiInfo;
                    
                    wifiInfo.intName = splitData[0];
                    wifiInfo.linkQuality = boost::lexical_cast<float>(
                            splitData[2]);
                    wifiInfo.signalLevel = boost::lexical_cast<float>(
                            splitData[3]);
                    wifiInfo.noiseLevel = boost::lexical_cast<float>(
                            splitData[4]);
                    
                    interfacesData.push_back(wifiInfo);
                }
            }
        }
    }
    
    stream.close();
    
    return true;
}

std::vector<WiFiSignalInfo> WiFiInfoParser::getInfo()
{
    return interfacesData;
}

void WiFiInfoParser::run()
{
    auto delayTime = std::chrono::milliseconds(1000);
    auto t = std::chrono::high_resolution_clock::now();

    while (!stopVariable)
    {
        if (readInfo())
        {
            cs::SensorData sensorMessage;

            sensorMessage.set_sensor_id((int) SensorIds::WIFI_sensor);
            sensorMessage.set_sensor_desc(WIFI_SENSOR_NAME);
            auto values = sensorMessage.mutable_sensor_values();

            auto info = getInfo();
            
            for (size_t i = 0; i < info.size(); i++)
            {
                auto sensorData = values->Add();
                
                sensorData->set_associated_name(WIFI_INTERFACE_NAME);
                sensorData->set_data_type(cs::WIFI_INFO);
                
                auto wifiInfo = sensorData->mutable_wifi_info();
                wifiInfo->set_interface_name(info[i].intName);
                wifiInfo->set_link_quality(info[i].linkQuality);
                wifiInfo->set_signal_level(info[i].signalLevel);
            }

            int messageSize = sensorMessage.ByteSize();
            std::vector<int8_t> outArray(messageSize);
            sensorMessage.SerializeToArray(&outArray[0], messageSize);
            
            sendData(outArray);
        }
        
        std::this_thread::sleep_until(t + delayTime);
        t += delayTime;
    }
}
