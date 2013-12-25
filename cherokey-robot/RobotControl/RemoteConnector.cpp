/* 
 * File:   RemoteConnector.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 19, 2013, 1:17 AM
 */

#include "RemoteConnector.hpp"
#include "common.pb.h"

#include <iostream>
#include <QMutexLocker>

namespace cc = cherokey::common;

#define PING_INTERVAL           1000
#define FAIL_CONN_TIMEOUT       3000

RemoteConnector::RemoteConnector(QObject *parent) 
: QThread(parent)
, started(false)
, pingTimer(this)
, queueMutex(new QMutex(QMutex::Recursive))
, failTimer(this)
{
    pingTimer.setInterval(PING_INTERVAL);
    
    failTimer.setSingleShot(true);
    failTimer.setInterval(FAIL_CONN_TIMEOUT);
    
    connect(&pingTimer, SIGNAL(timeout()), SLOT(onPingTimeout()));
    connect(&failTimer, SIGNAL(timeout()), SLOT(onPingFail()));
}

RemoteConnector::~RemoteConnector() 
{
    
}

void RemoteConnector::run()
{
    try
    {
        socketPtr->connect(serverUri.toStdString().c_str());

        pingTimer.start();
        failTimer.start(failTimer.interval());

        while (started)
        {
            {
                QMutexLocker lock(queueMutex.data());
                while (!commandQueue.empty())
                {
                    QSharedPointer<SocketCommand> commandItem = 
                            commandQueue.dequeue();
                    lock.unlock();

                    if (commandItem != NULL)
                    {
                        if (!commandItem->doCommand(*socketPtr))
                        {
                            std::cout << "Failed to handle command" << 
                                    std::endl;
                        }
                    }
                    
                    lock.relock();
                }
            }
            
            msleep(1);
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
    
    pingTimer.stop();
    failTimer.stop();
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
    pingSeqno = 0;
    commandQueue.clear();
    
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

void RemoteConnector::onPingTimeout()
{
    QSharedPointer<SocketCommand> pingCommand(new PingCommand(
        failTimer, pingSeqno++));
    handleCommand(pingCommand);
}

void RemoteConnector::handleCommand(QSharedPointer<SocketCommand>& commandPtr)
{
    QMutexLocker lock(queueMutex.data());
    commandQueue.enqueue(commandPtr);
}

void RemoteConnector::onPingFail()
{
    emit ConversationTerminated("No answer to PING request");
}
