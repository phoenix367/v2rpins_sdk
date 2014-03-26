/* 
 * File:   CommandSender.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 9, 2014, 11:48 PM
 */

#ifndef COMMANDSENDER_HPP
#define	COMMANDSENDER_HPP

#include <zmq.hpp>

#include "AbstractSender.hpp"

namespace cherokey
{
    namespace common
    {
        class CommandMessage;
    }
    
    namespace notifications
    {
        class NotificationMessage;
    }
}

class CommandSender : public AbstractSender<cherokey::common::CommandMessage>
{
public:
    CommandSender();
    virtual ~CommandSender();
};

class NotifySender : public AbstractSender<
        cherokey::notifications::NotificationMessage>
{
public:
    NotifySender();
    virtual ~NotifySender();
};

#endif	/* COMMANDSENDER_HPP */
