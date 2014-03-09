/* 
 * File:   CommandSender.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on March 9, 2014, 11:48 PM
 */

#include "CommandSender.hpp"
#include "common.pb.h"
#include "Exceptions.hpp"

extern zmq::context_t gContext;

CommandSender::CommandSender(const std::string& connectAddr) 
: socket(gContext, ZMQ_PUSH)
{
    try
    {
        socket.connect(connectAddr.c_str());
    }
    catch (zmq::error_t& e)
    {
        COMM_EXCEPTION(InternalError, e.what());
    }
}

CommandSender::~CommandSender() 
{
    socket.close();
}

void CommandSender::sendMessage(const cherokey::common::CommandMessage& msg)
{
    int messageSize = msg.ByteSize();
    std::vector<int8_t> outArray(messageSize);
    msg.SerializeToArray(&outArray[0], messageSize);
    
    socket.send(&outArray[0], messageSize, 0);
}
