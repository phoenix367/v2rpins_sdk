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
    contextPtr = QSharedPointer<zmq::context_t>(new zmq::context_t(1));
}

RemoteConnector::~RemoteConnector() 
{
    
}

void RemoteConnector::run()
{
    zmq::socket_t socket(*contextPtr, ZMQ_REQ);
    socket.connect(serverUri.toStdString().c_str());
    
    zmq::message_t message;
    qint64 seqno = 0;
    
    while (started)
    {
        cherokey::common::Ping pingMsg;
        pingMsg.set_seqno(seqno);

        int messageSize = pingMsg.ByteSize();
        std::vector<uint8_t> outArray(messageSize);
        pingMsg.SerializeToArray(&outArray[0], messageSize);

        socket.send(&outArray[0], messageSize);
        socket.recv(&message);
        
        sleep(1);
        seqno++;
    }
}

void RemoteConnector::connectToServer(const QString& uri)
{
    if (started)
    {
        started = false;
        terminate();
    }
    
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
    contextPtr.clear();
}
