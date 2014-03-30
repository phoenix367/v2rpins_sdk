/* 
 * File:   NotificationHelper.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on 30 Март 2014 г., 18:55
 */

#include "NotificationHelper.hpp"
#include "Exceptions.hpp"

extern zmq::context_t gContext;

NotificationHelper::NotificationHelper()
: notificationSocket(gContext, ZMQ_PUSH)
, connected(false)
{
}

NotificationHelper::~NotificationHelper() 
{
    notificationSocket.close();
}

void NotificationHelper::connect(const std::string& addr)
{
    notificationSocket.connect(addr.c_str());
    endPoint = addr;
    connected = true;
}

void NotificationHelper::disconnect()
{
    if (connected)
    {
        int r = zmq_disconnect(notificationSocket, endPoint.c_str());
        
        if (r != 0)
        {
            int e = zmq_errno();
            const char* errorStr = zmq_strerror(e);
            
            COMM_EXCEPTION(InternalError, errorStr);
        }
        
        connected = false;
    }
}

void NotificationHelper::sendMessage(zmq::message_t& msg)
{
    notificationSocket.send(msg);
}

bool NotificationHelper::isConnected()
{
    return connected;
}
