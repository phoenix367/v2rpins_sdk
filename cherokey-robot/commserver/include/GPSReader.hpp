/* 
 * File:   GPSReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 3, 2014, 9:51 PM
 */

#ifndef GPSREADER_HPP
#define	GPSREADER_HPP

#include "serialstream.h"
#include "nmea/nmea.h"
#include "Exceptions.hpp"
#include "SensorReader.hpp"

class GPSReader : public SensorReader
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
    
protected:
    virtual void run();

private:
    std::shared_ptr<SerialStream> serialStreamPtr;
};

#endif	/* GPSREADER_HPP */
