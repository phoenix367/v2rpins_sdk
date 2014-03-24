/* 
 * File:   PIDCommands.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 23, 2014, 5:10 PM
 */

#ifndef PIDCOMMANDS_HPP
#define	PIDCOMMANDS_HPP

#include <type_traits>

#include "IMUReader.hpp"

enum class CommandType
{
    rotating
};

class IPIDCommand
{
public:
    IPIDCommand();
    virtual ~IPIDCommand();
    
    virtual CommandType getCommandType() = 0;

    template <typename T> T* commandInstance()
    {
        static_assert(std::is_base_of<IPIDCommand, T>::value,
                "Type of convertion should be derived from IPIDCommand");
        return (T*) this;
    }
};

class RotateCommand : public IPIDCommand
{
public:
    RotateCommand(float angle);
    virtual ~RotateCommand();
    
    virtual CommandType getCommandType();
    float getRotationAngle();
    
private:
    float rotationAngle;
};

#endif	/* PIDCOMMANDS_HPP */
