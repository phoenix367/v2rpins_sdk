/* 
 * File:   RemoteConnector.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 19, 2013, 1:17 AM
 */

#include "RemoteConnector.hpp"
#include "common.pb.h"

#include <iostream>

namespace cc = cherokey::common;

RemoteConnector::RemoteConnector(QObject *parent) 
: QThread(parent)
, started(false)
{
}

RemoteConnector::~RemoteConnector() 
{
    
}

void RemoteConnector::run()
{
    try
    {
        socketPtr->connect(serverUri.toStdString().c_str());

        zmq::message_t message;
        qint64 seqno = 0;

        while (started)
        {
            cc::CommandMessage commandMessage;
            commandMessage.set_type(cc::CommandMessage::PING);
            cc::Ping* pingMsg = commandMessage.mutable_ping();
            pingMsg->set_seqno(seqno);

            int messageSize = commandMessage.ByteSize();
            std::vector<uint8_t> outArray(messageSize);
            commandMessage.SerializeToArray(&outArray[0], messageSize);

            zmq::message_t outMessage(messageSize);
            memcpy(outMessage.data(), &outArray[0], messageSize);
            if (!socketPtr->send(outMessage))
            {
                
            }
            
            if (!socketPtr->recv(&message))
            {
                std::cout << "Failed to receive message from remote host" <<
                        std::endl;
            }

            sleep(1);
            seqno++;
        }
    }
    catch (zmq::error_t& e)
    {
        std::cout << "Exception occured: " << e.what() <<
                        std::endl;
        
        if (e.num() != ETERM)
        {
            emit ConversationTerminated(e.what());
        }
    }
}

void RemoteConnector::connectToServer(const QString& uri)
{
    if (started)
    {
        return;
    }
    
    contextPtr = QSharedPointer<zmq::context_t>(new zmq::context_t(1));
    socketPtr = QSharedPointer<zmq::socket_t>(new zmq::socket_t(
            *contextPtr, ZMQ_REQ));
    
    int lingerValue = 0;
    socketPtr->setsockopt(ZMQ_LINGER, &lingerValue, sizeof(int));
    started = true;
    serverUri = uri;
    start();
}

void RemoteConnector::disconnectFromServer()
{
    if (!started)
    {
        return;
    }
    
    started = false;
    socketPtr->close();
    contextPtr.clear();
    
    wait();
}
