/* 
 * File:   ConnectionListener.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 18, 2013, 9:17 PM
 */

#ifndef CONNECTIONLISTENER_HPP
#define	CONNECTIONLISTENER_HPP

#include "ConfigManager.hpp"
#include "common.pb.h"
#include <zmq.hpp>

class ConnectionListener 
{
public:
    ConnectionListener(std::shared_ptr<ConnectionInfo>& infoPtr);
    virtual ~ConnectionListener();
    
    void run();
    
private:
    void processPing(zmq::socket_t& socket, 
            const cherokey::common::CommandMessage& msg);
    
private:
    std::shared_ptr<ConnectionInfo> connectionParams;
};

#endif	/* CONNECTIONLISTENER_HPP */

