/* 
 * File:   RemoteConnector.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 19, 2013, 1:17 AM
 */

#include "RemoteConnector.hpp"
#include "common.pb.h"

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
            cherokey::common::Ping pingMsg;
            pingMsg.set_seqno(seqno);

            int messageSize = pingMsg.ByteSize();
            std::vector<uint8_t> outArray(messageSize);
            pingMsg.SerializeToArray(&outArray[0], messageSize);

            zmq::message_t outMessage(messageSize);
            memcpy(outMessage.data(), &outArray[0], messageSize);
            if (!socketPtr->send(outMessage))
            {
                
            }
            
            if (!socketPtr->recv(&message))
            {
                
            }

            sleep(1);
            seqno++;
        }
    }
    catch (std::exception& e)
    {
        
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
