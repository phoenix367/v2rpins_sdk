/* 
 * File:   SensorsConnector.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 29, 2013, 11:53 PM
 */

#ifndef SENSORSCONNECTOR_HPP
#define	SENSORSCONNECTOR_HPP

#include <QThread>
#include <zmq.hpp>
#include <QSharedPointer>

class SensorsConnector : public QThread
{
    Q_OBJECT
    
public:
    SensorsConnector(QObject *parent = NULL);
    virtual ~SensorsConnector();
    
    void startSubscriber(const QString& url);
    void stopSubscriber();
    
public:
    Q_SIGNAL void VoltageData(float voltage, float current);
    
protected:
    virtual void run();

private:
    volatile bool started;
    QSharedPointer<zmq::socket_t> socketPtr;
};

#endif	/* SENSORSCONNECTOR_HPP */
