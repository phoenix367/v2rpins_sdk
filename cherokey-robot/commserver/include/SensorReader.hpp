/* 
 * File:   SensorReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 14, 2014, 12:31 AM
 */

#ifndef SENSORREADER_HPP
#define	SENSORREADER_HPP

#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>
#include <memory>
#include <zmq.hpp>
#include <vector>

class SensorReader 
{
public:
    SensorReader();
    virtual ~SensorReader();
    
protected:
    virtual void run() = 0;
    void initThread();
    void stopThread();
    
    void sendData(const std::vector<int8_t>& data);
    
private:
    std::unique_ptr<boost::thread> readerThread;
    
protected:
    boost::atomic<bool> stopVariable;
    std::unique_ptr<zmq::socket_t> socket;
};

#endif	/* SENSORREADER_HPP */
