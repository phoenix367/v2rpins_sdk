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

#include "Globals.hpp"

struct GPSInfo
{
    double latitude;
    double longitude;
};

class SensorsConnector : public QThread
{
    Q_OBJECT
    
public:
    SensorsConnector(QObject *parent = NULL);
    virtual ~SensorsConnector();
    
    void startSubscriber(const ConnectionInfo& info);
    void stopSubscriber();
    
public:
    Q_SIGNAL void VoltageData(float voltage, float current);
    Q_SIGNAL void GPSData(GPSInfo gpsInfo);
    Q_SIGNAL void ModelRotation(float angleX, float angleY, float angleZ);
    Q_SIGNAL void ready();
    Q_SIGNAL void cmdResult(quint64 cmdIdx, bool result);
    Q_SIGNAL void WiFiData(QString interfaceName, float quality, 
        float sigLevel);
    
protected:
    virtual void run();

private:
    volatile bool started;
    QSharedPointer<zmq::socket_t> socketPtr;
    QSharedPointer<zmq::socket_t> socketNotifyPtr;
    ConnectionInfo connectionInfo;
};

#endif	/* SENSORSCONNECTOR_HPP */
