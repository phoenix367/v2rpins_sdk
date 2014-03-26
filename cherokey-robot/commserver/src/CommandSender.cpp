/* 
 * File:   CommandSender.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on March 9, 2014, 11:48 PM
 */

#include "CommandSender.hpp"
#include "ConnectionListener.hpp"

CommandSender::CommandSender() 
: AbstractSender(ConnectionListener::INTERNAL_COMMAND_ADDR)
{

}

CommandSender::~CommandSender() 
{

}

NotifySender::NotifySender() 
: AbstractSender(ConnectionListener::INTERNAL_NOTIFY_ADDR)
{

}

NotifySender::~NotifySender() 
{

}
