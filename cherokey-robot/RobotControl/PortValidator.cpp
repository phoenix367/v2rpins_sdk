/* 
 * File:   PortValidator.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 3, 2014, 1:38 AM
 */

#include "PortValidator.hpp"

PortValidator::PortValidator(QObject *parent)
: QValidator(parent)
{
}

PortValidator::~PortValidator() 
{
}

void PortValidator::fixup(QString &) const
{
    
}

PortValidator::State PortValidator::validate(QString &input, int &) const
{
    if (input.isEmpty()) 
    {
        return Acceptable;
    }
    
    bool ok;
    
    int val = input.toInt(&ok);
    if (!ok || val < 0 || val > 65535) 
    {
        return Invalid;
    }
    
    return Acceptable;
}
