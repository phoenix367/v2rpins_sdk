/* 
 * File:   OdometrySink.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 4 Март 2014 г., 22:54
 */

#ifndef ODOMETRYSINK_HPP
#define	ODOMETRYSINK_HPP

#include <QGst/Object>
#include <QGst/Utils/ApplicationSink>

class OdometrySink : public QGst::Utils::ApplicationSink
{
public:
    OdometrySink();
    virtual ~OdometrySink();
    
protected:
    
private:

};

#endif	/* ODOMETRYSINK_HPP */
