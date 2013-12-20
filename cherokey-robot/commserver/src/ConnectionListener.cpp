/* 
 * File:   ConnectionListener.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 18, 2013, 9:17 PM
 */

#include "ConnectionListener.hpp"
#include "Exceptions.hpp"

namespace cc = cherokey::common;

ConnectionListener::ConnectionListener(
    std::shared_ptr<ConnectionInfo>& infoPtr) 
{
    if (infoPtr == nullptr)
    {
        COMM_EXCEPTION(NullPointerException, "Connection info is null.");
    }
    
    connectionParams = infoPtr;
}

ConnectionListener::~ConnectionListener() 
{
}

void ConnectionListener::run()
{
    std::ostringstream stream;
    
    stream << "tcp://" << connectionParams->ipAddress.to_string() << ":" <<
            connectionParams->port;
    // Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    
    std::cout << stream.str() << std::endl;
    
    socket.bind(stream.str().c_str());
    
    while (true)
    {
        zmq::message_t message;
        socket.recv(&message);
        
        cc::CommandMessage commandMsg;
        if (!commandMsg.ParseFromArray(message.data(), message.size()))
        {
            std::cout << "Invalid message" << std::endl;
        }
        else
        {
            switch (commandMsg.type())
            {
                case cc::CommandMessage::PING:
                    processPing(socket, commandMsg);
                    break;
                default:
                    std::cout << "Received unknown message with type " <<
                          commandMsg.type() << std::endl;  
            }
        }
    }
}

void ConnectionListener::processPing(zmq::socket_t& socket, 
        const cherokey::common::CommandMessage& msg)
{
    auto pingMsg = msg.ping();
    auto seqno = pingMsg.seqno();

    std::cout << "Received ping message with seqno=" << seqno <<
            std::endl;
    cc::CommandReply replyMsg;
    replyMsg.set_type(cc::CommandReply::PONG);
    
    auto pongMsg = replyMsg.mutable_pong();
    pongMsg->set_seqno(seqno);

    int messageSize = replyMsg.ByteSize();
    std::vector<uint8_t> outArray(messageSize);
    replyMsg.SerializeToArray(&outArray[0], messageSize);

    socket.send(&outArray[0], messageSize);
    std::cout << "Sent pong message with seqno=" << seqno << std::endl;
}
