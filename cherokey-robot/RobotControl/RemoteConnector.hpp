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

class RemoteConnector : public QThread
{
    Q_OBJECT
    
public:
    RemoteConnector(QObject *parent = NULL);
    virtual ~RemoteConnector();
    
    void connectToServer(const QString& uri);
    void disconnectFromServer();
    void handleCommand(QSharedPointer<SocketCommand>& commandPtr);
    
public:
    Q_SIGNAL void ConversationTerminated(const QString& msg);
    
private:
    Q_SLOT void onPingTimeout();
    
protected:
    virtual void run();
    
private:
    QSharedPointer<zmq::context_t> contextPtr;
    bool started;
    QString serverUri;
    QSharedPointer<zmq::socket_t> socketPtr;
    QTimer pingTimer;
    QQueue<QSharedPointer<SocketCommand> > commandQueue;
    QSharedPointer<QMutex> queueMutex;
};

#endif	/* REMOTECONNECTOR_HPP */
