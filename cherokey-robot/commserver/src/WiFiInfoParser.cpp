/* 
 * File:   WiFiInfoParser.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on June 1, 2014, 1:04 AM
 */

#include "WiFiInfoParser.hpp"

#include <fstream>
#include <iostream>

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
    
    while (!stream.eof())
    {
        std::string strLine;
        std::getline(stream, strLine);
        
        if (!strLine.empty())
        {
            std::cout << strLine << std::endl;
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
            auto info = getInfo();
            
            if (!info.empty())
            {
                
            }
        }
        
        std::this_thread::sleep_until(t + delayTime);
        t += delayTime;
    }
}
