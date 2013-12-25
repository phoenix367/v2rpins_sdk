/* 
 * File:   SensorsListener.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 25 Декабрь 2013 г., 15:40
 */

#ifndef SENSORSLISTENER_HPP
#define	SENSORSLISTENER_HPP

#include <QThread>

class SensorsListener : public QThread
{
    Q_OBJECT
    
public:
    SensorsListener();
    virtual ~SensorsListener();
    
public:
    Q_SIGNAL void SensorsData();
    
protected:
    virtual void run();

private:

};

#endif	/* SENSORSLISTENER_HPP */
