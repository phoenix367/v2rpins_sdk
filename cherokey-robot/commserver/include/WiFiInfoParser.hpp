/* 
 * File:   WiFiInfoParser.hpp
 * Author: Ivan Gubochkin
 *
 * Created on June 1, 2014, 1:04 AM
 */

#ifndef WIFIINFOPARSER_HPP
#define	WIFIINFOPARSER_HPP

#include <string>
#include <vector>
#include "SensorReader.hpp"

struct WiFiSignalInfo
{
    std::string intName;
    float linkQuality;
    float signalLevel;
    float noiseLevel;
};

class WiFiInfoParser : public SensorReader
{
public:
    WiFiInfoParser();
    virtual ~WiFiInfoParser();
    
    bool readInfo();
    std::vector<WiFiSignalInfo> getInfo();
    
private:
    void run();
    
private:
    static const std::string WIRELESS_INFO_FILE_NAME;
    
    std::vector<WiFiSignalInfo> interfacesData;
};

#endif	/* WIFIINFOPARSER_HPP */
