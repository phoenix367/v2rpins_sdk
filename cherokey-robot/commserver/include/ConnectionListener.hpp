/* 
 * File:   ConnectionListener.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 18, 2013, 9:17 PM
 */

#ifndef CONNECTIONLISTENER_HPP
#define	CONNECTIONLISTENER_HPP

#include "ConfigManager.hpp"

class ConnectionListener 
{
public:
    ConnectionListener(std::shared_ptr<ConnectionInfo>& infoPtr);
    virtual ~ConnectionListener();
    
    void run();
    
private:
    std::shared_ptr<ConnectionInfo> connectionParams;
};

#endif	/* CONNECTIONLISTENER_HPP */

