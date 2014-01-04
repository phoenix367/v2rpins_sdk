/* 
 * File:   GPSReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 3, 2014, 9:51 PM
 */

#include "GPSReader.hpp"
#include "ConfigManager.hpp"

#include <iostream>
#include <iomanip>

GPSReader::GPSReader()
: stopVariable(false)
{
    auto instance = ConfigManager::getInstance();
    
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Pointer to instance of "
                "configuration manager is null.");
    }
    
    SerialOptions options;
    options.setDevice(instance->getGPSDevice());
    options.setBaudrate(instance->getGPSDeviceBaudrate());
    options.setTimeout(boost::posix_time::seconds(3));
    
    serialStreamPtr = std::shared_ptr<SerialStream>(
            new SerialStream(options));
    serialStreamPtr->exceptions(std::ios::badbit | std::ios::failbit);
    
    readerThread = std::unique_ptr<boost::thread>(
                    new boost::thread(
                    boost::bind(&GPSReader::run, this)));
}

GPSReader::~GPSReader() 
{
    stopVariable = true;
    readerThread->join();
}

void GPSReader::run()
{
    ParserHolder parser;
    nmeaINFO info;
    
    nmea_zero_INFO(&info);
    nmea_parser_init(parser);
    
    double degLatPrev = INFINITY, degLonPrev = INFINITY;
    
    while (!stopVariable)
    {
        std::string s;
        
        *serialStreamPtr >> s;
        
        s += "\r\n";
        nmea_parse(parser, s.c_str(), s.size(), &info);
        
        double degLat = nmea_ndeg2degree(info.lat);
        double degLon = nmea_ndeg2degree(info.lon);
        
        if ((degLat != degLatPrev || degLon != degLonPrev) &&
                info.sig != 0)
        {
            degLatPrev = degLat;
            degLonPrev = degLon;
            
            std::cout << info.smask << " " << 
                    std::setprecision(10) << degLat << " " << degLon << " " <<
                    info.sig << std::endl;
        }
    }
}
