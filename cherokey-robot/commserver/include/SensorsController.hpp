/* 
 * File:   SensorsController.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 27, 2013, 3:06 PM
 */

#ifndef SENSORSCONTROLLER_HPP
#define	SENSORSCONTROLLER_HPP

#include <memory>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <zmq.hpp>

#include "ConfigManager.hpp"
#include "pincontroller/pincontroller.hpp"

class SensorsController 
{
public:
    static const char* SENSORS_CONN_POINT;
    
public:
    SensorsController();
    virtual ~SensorsController();
    
    static SensorsController* getInstance();
    
    void startPublisher(const ConnectionInfo& info);
    void stopPublisher();
    
    void putMessage(const std::vector<int8_t>& msg);
    
private:
    void run();
    void processSensorMessages(zmq::socket_t& pubSocket,
            zmq::socket_t& sensorSocket);
    bool getMessage(std::vector<int8_t>& msg);

private:
    static std::unique_ptr<SensorsController> instance;
    
    std::unique_ptr<std::thread> producerThread;
    bool started;
    std::atomic<bool> stopVariable;
    pc::ADCReader adcReader;
    ConnectionInfo connectionInfo;
    
    std::queue<std::vector<int8_t>> messageQueue;
    std::mutex queueMutex;
};

#endif	/* SENSORSCONTROLLER_HPP */
