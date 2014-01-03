/* 
 * File:   GPSReader.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 3, 2014, 9:51 PM
 */

#include "GPSReader.hpp"

#include <iostream>

GPSReader::GPSReader()
: stopVariable(false)
{
    SerialOptions options;
    options.setDevice("/dev/ttyS1");
    options.setBaudrate(115200);
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
    while (!stopVariable)
    {
        std::string s;
        
        *serialStreamPtr >> s;
        std::cout << "Read " << s.size() << " bytes from GPS receiver" << 
                std::endl;
    }
}
