/* 
 * File:   OdometrySink.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on 4 Март 2014 г., 22:54
 */

#include "OdometrySink.hpp"
#include <QGst/Buffer>

OdometrySink::OdometrySink() 
{
}

OdometrySink::~OdometrySink() 
{
}

void OdometrySink::eos()
{
}

QGst::FlowReturn OdometrySink::newBuffer()
{
    QGst::BufferPtr buffer = pullBuffer();
    if (buffer)
    {
        
    }
    
    return QGst::FlowOk;
}
