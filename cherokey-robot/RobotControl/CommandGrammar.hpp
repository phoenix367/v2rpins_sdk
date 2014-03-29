/* 
 * File:   CommandGrammare.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 28, 2014, 11:35 PM
 */

#ifndef COMMANDGRAMMARE_HPP
#define	COMMANDGRAMMARE_HPP

#include "Commands.hpp"

#include <vector>
#include <QSharedPointer>

bool parseCommandStr(const std::string& str, 
        QSharedPointer<SocketCommand>& cmd);

#endif	/* COMMANDGRAMMARE_HPP */
