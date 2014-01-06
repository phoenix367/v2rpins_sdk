/* 
 * File:   SensorsController.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 27, 2013, 3:06 PM
 */

#ifndef SENSORSCONTROLLER_HPP
#define	SENSORSCONTROLLER_HPP

#include <memory>
#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>
#include <zmq.hpp>

#include "ConfigManager.hpp"
#include "pincontroller/pincontroller.hpp"

class SensorsController 
{
public:
    SensorsController();
    virtual ~SensorsController();
    
    static SensorsController* getInstance();
    
    void startPublisher(const ConnectionInfo& info);
    void stopPublisher();
    
private:
    void run();
    void processSensorMessages(zmq::socket_t& pubSocket,
            zmq::socket_t& sensorSocket);

private:
    static std::unique_ptr<SensorsController> instance;
    
    std::unique_ptr<boost::thread> producerThread;
    bool started;
    boost::atomic<bool> stopVariable;
    pc::ADCReader adcReader;
    ConnectionInfo connectionInfo;
};

#endif	/* SENSORSCONTROLLER_HPP */
