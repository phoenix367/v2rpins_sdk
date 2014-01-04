/* 
 * File:   GPSReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 3, 2014, 9:51 PM
 */

#ifndef GPSREADER_HPP
#define	GPSREADER_HPP

#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>

#include "serialstream.h"
#include "nmea/nmea.h"
#include "Exceptions.hpp"

class GPSReader 
{
private:
    class ParserHolder
    {
    public:
        ParserHolder()
        {
            if (!nmea_parser_init(&parser))
            {
                COMM_EXCEPTION(InternalError, "Failed to create NMEA parser "
                    "instance");
            }
        }
        
        ~ParserHolder()
        {
            nmea_parser_destroy(&parser);
        }
        
        operator nmeaPARSER*()
        {
            return &parser;
        }

    private:
        nmeaPARSER parser;
    };
    
public:
    GPSReader();
    virtual ~GPSReader();
    
private:
    void run();

private:
    std::shared_ptr<SerialStream> serialStreamPtr;
    std::unique_ptr<boost::thread> readerThread;
    boost::atomic<bool> stopVariable;
};

#endif	/* GPSREADER_HPP */
