/* 
 * File:   NotificationHelper.hpp
 * Author: Ivsan Gubochkin
 *
 * Created on 30 Март 2014 г., 18:55
 */

#ifndef NOTIFICATIONHELPER_HPP
#define	NOTIFICATIONHELPER_HPP

#include <zmq.hpp>

class NotificationHelper 
{
public:
    NotificationHelper();
    virtual ~NotificationHelper();

    void connect(const std::string& addr);
    void disconnect();
    void sendMessage(zmq::message_t& msg);
    bool isConnected();
    
private:
    zmq::socket_t notificationSocket;
    bool connected;
    std::string endPoint;
};

#endif	/* NOTIFICATIONHELPER_HPP */
