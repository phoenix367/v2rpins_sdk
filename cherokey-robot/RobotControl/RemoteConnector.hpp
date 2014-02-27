/* 
 * File:   RemoteConnector.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 19, 2013, 1:17 AM
 */

#ifndef REMOTECONNECTOR_HPP
#define	REMOTECONNECTOR_HPP

#include <QThread>
#include <QSharedPointer>
#include <QQueue>
#include <QTimer>
#include <QMutex>
#include <zmq.hpp>

#include "Commands.hpp"
#include "SensorsConnector.hpp"

class RemoteConnector : public QThread
{
    Q_OBJECT
    
public:
    RemoteConnector(QObject *parent = NULL);
    virtual ~RemoteConnector();
    
    void connectToServer(const ConnectionInfo& info);
    void disconnectFromServer();
    void handleCommand(QSharedPointer<SocketCommand>& commandPtr);
    
public:
    Q_SIGNAL void ConversationTerminated(const QString& msg);
    Q_SIGNAL void CommandSuccess(int commandType, uint64_t commandIndex);
    
private:
    Q_SLOT void onPingTimeout();
    Q_SLOT void onPingFail();
    
protected:
    virtual void run();
    
private:
    volatile bool started;
    ConnectionInfo connectionInfo;
    QSharedPointer<zmq::socket_t> socketPtr;
    QTimer pingTimer;
    QQueue<QSharedPointer<SocketCommand> > commandQueue;
    QSharedPointer<QMutex> queueMutex;
    QTimer failTimer;
    int64_t pingSeqno;
    QSharedPointer<SensorsConnector> sensorsConnector;
};

#endif	/* REMOTECONNECTOR_HPP */
