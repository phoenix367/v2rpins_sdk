/* 
 * File:   Globals.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 3, 2014, 1:33 AM
 */

#ifndef GLOBALS_HPP
#define	GLOBALS_HPP

#include <QString>

struct ConnectionInfo
{
    QString ipAddress;
    int commandPort;
    int sensorsPort;
};

quint32 getHostAddress();

#endif	/* GLOBALS_HPP */
