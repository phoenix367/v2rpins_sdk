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
    rotating,
    wait,
    moveTime
};

class IPIDCommand
{
public:
    IPIDCommand(uint64_t id);
    virtual ~IPIDCommand();
    
    virtual CommandType getCommandType() = 0;

    template <typename T> T* commandInstance()
    {
        static_assert(std::is_base_of<IPIDCommand, T>::value,
                "Type of convertion should be derived from IPIDCommand");
        return (T*) this;
    }
    
    uint64_t getCommandId();
    
private:
    uint64_t commandId;
};

class RotateCommand : public IPIDCommand
{
public:
    RotateCommand(uint64_t id, float angle);
    virtual ~RotateCommand();
    
    virtual CommandType getCommandType();
    float getRotationAngle();
    
private:
    float rotationAngle;
};

class WaitCommand : public IPIDCommand
{
public:
    WaitCommand(uint64_t id, float d);
    virtual ~WaitCommand();
    
    virtual CommandType getCommandType();
    float getDuration();
    
private:
    float duration;
};

class MoveTimeCommand : public IPIDCommand
{
public:
    MoveTimeCommand(uint64_t id, float d, bool r);
    virtual ~MoveTimeCommand();
    
    virtual CommandType getCommandType();
    float getDuration();
    bool getDirection();
    
private:
    float duration;
    bool direction;
};

#endif	/* PIDCOMMANDS_HPP */
