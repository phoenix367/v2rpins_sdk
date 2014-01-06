/* 
 * File:   GPSReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 3, 2014, 9:51 PM
 */

#include "GPSReader.hpp"
#include "ConfigManager.hpp"
#include "sensors.pb.h"

#include <iostream>
#include <iomanip>
#include <zmq.hpp>

extern zmq::context_t gContext;

namespace cs = cherokey::sensors;

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
    
    zmq::socket_t socket(gContext, ZMQ_PUSH);
    socket.connect("inproc://sensors");
    
    double degLatPrev = INFINITY, degLonPrev = INFINITY;
    
    cs::SensorData sensorMessage;

    sensorMessage.set_sensor_id(1);
    sensorMessage.set_sensor_desc("GPS");
    auto values = sensorMessage.mutable_sensor_values();
    auto latData = values->Add();
    auto lonData = values->Add();
    
    latData->set_associated_name("latitude");
    latData->set_data_type(cs::REAL);
    lonData->set_associated_name("longitude");
    lonData->set_data_type(cs::REAL);

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
            
            latData->set_real_value(degLat);
            lonData->set_real_value(degLon);

            int messageSize = sensorMessage.ByteSize();
            std::vector<int8_t> outArray(messageSize);
            sensorMessage.SerializeToArray(&outArray[0], messageSize);

            socket.send(&outArray[0], messageSize);
        }
    }
    
    socket.close();
}
