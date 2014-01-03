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

class GPSReader 
{
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
