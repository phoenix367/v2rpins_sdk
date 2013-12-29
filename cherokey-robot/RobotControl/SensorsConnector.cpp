/* 
 * File:   SensorsConnector.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 29, 2013, 11:53 PM
 */

#include "SensorsConnector.hpp"
#include "messages/sensors.pb.h"

#include <iostream>

namespace cs = cherokey::sensors;

extern zmq::context_t gContext;

SensorsConnector::SensorsConnector(QObject* parent) 
: QThread(parent)
, started(false)
{
}

SensorsConnector::~SensorsConnector() 
{
}

void SensorsConnector::run()
{
    try
    {
        socketPtr->connect("tcp://192.168.1.128:1231");
        
        while (true)
        {
            zmq::message_t msg;

            zmq::pollitem_t items[] = { { *socketPtr, 0, ZMQ_POLLIN, 0 } };
            zmq::poll(items, 1, 1000);

            if (items[0].revents & ZMQ_POLLIN) 
            {
                if (socketPtr->recv(&msg))
                {
                    cs::SensorData dataMsg;
                    dataMsg.ParseFromArray(msg.data(), msg.size());
                    
                    float voltage = 0, current = 0;
                    
                    for (int i = 0; i < dataMsg.sensor_values_size(); i++)
                    {
                        cs::SensorValue value = dataMsg.sensor_values(i);
                        
                        if (value.associated_name() == "voltage" &&
                            value.has_real_value())
                        {
                            voltage = value.real_value();
                        }
                        else if (value.associated_name() == "current" &&
                            value.has_real_value())
                        {
                            current = value.real_value();
                        }
                    }
                    
                    VoltageData(voltage, current);
                }
            }
        }
    }
    catch (zmq::error_t& e)
    {
        std::cout << "Exception occured: " << e.what() <<
                        std::endl;
    }
}

void SensorsConnector::startSubscriber(const QString& url)
{
    socketPtr = QSharedPointer<zmq::socket_t>(
            new zmq::socket_t(gContext, ZMQ_SUB));
    socketPtr->setsockopt(ZMQ_SUBSCRIBE, NULL, 0);

    int lingerValue = 0;
    socketPtr->setsockopt(ZMQ_LINGER, &lingerValue, sizeof(int));
    
    start();
}

void SensorsConnector::stopSubscriber()
{
    socketPtr->close();
    wait();
}
