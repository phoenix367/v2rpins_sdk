/* 
 * File:   CommandSender.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 9, 2014, 11:48 PM
 */

#ifndef COMMANDSENDER_HPP
#define	COMMANDSENDER_HPP

#include <zmq.hpp>

namespace cherokey
{
    namespace common
    {
        class CommandMessage;
    }
}

class CommandSender 
{
public:
    CommandSender(const std::string& connectAddr);
    virtual ~CommandSender();
    
protected:
    void sendMessage(const cherokey::common::CommandMessage& msg);
    
private:
    zmq::socket_t socket;
};

#endif	/* COMMANDSENDER_HPP */
