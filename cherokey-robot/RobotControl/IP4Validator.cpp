/* 
 * File:   IP4Validator.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on January 3, 2014, 12:59 AM
 */

#include "IP4Validator.hpp"

#include <QStringList>

IP4Validator::IP4Validator(QObject *parent)
: QValidator(parent)
{
}

IP4Validator::~IP4Validator() 
{
}

void IP4Validator::fixup(QString &) const
{
    
}

IP4Validator::State IP4Validator::validate(QString &input, int &) const
{
    if (input.isEmpty()) 
    {
        return Acceptable;
    }
    
    QStringList slist = input.split(".");
    int s = slist.size();
    if (s > 4) 
    {
        return Invalid;
    }
    
    bool emptyGroup = false;
    
    for (int i = 0; i < s; i++)
    {
        bool ok;
        if(slist[i].isEmpty())
        {
            emptyGroup = true;
            continue;
        }
        
        int val = slist[i].toInt(&ok);
        if (!ok || val < 0 || val > 255) 
        {
            return Invalid;
        }
    }
    
    if (s < 4 || emptyGroup)
    {
        return Intermediate;
    }
    
    return Acceptable;
}
