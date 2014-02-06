/* 
 * File:   SensorReader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 14, 2014, 12:31 AM
 */

#ifndef SENSORREADER_HPP
#define	SENSORREADER_HPP

#include <thread>
#include <atomic>
#include <memory>
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
    std::unique_ptr<std::thread> readerThread;
    
protected:
    std::atomic<bool> stopVariable;
};

#endif	/* SENSORREADER_HPP */
