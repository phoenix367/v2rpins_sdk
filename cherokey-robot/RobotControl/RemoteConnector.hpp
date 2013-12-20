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
#include <zmq.hpp>

class RemoteConnector : public QThread
{
    Q_OBJECT
    
public:
    RemoteConnector(QObject *parent = NULL);
    virtual ~RemoteConnector();
    
    void connectToServer(const QString& uri);
    void disconnectFromServer();
    
protected:
    virtual void run();
    
private:
    QSharedPointer<zmq::context_t> contextPtr;
    bool started;
    QString serverUri;
    QSharedPointer<zmq::socket_t> socketPtr;
};

#endif	/* REMOTECONNECTOR_HPP */

