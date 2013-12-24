/* 
 * File:   Commands.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 23, 2013, 10:51 PM
 */

#ifndef COMMANDS_HPP
#define	COMMANDS_HPP

#include <zmq.hpp>
#include <QTimer>

#include "messages/common.pb.h"

namespace cc = cherokey::common;

class SocketCommand
{
public:
    virtual bool doCommand(zmq::socket_t& socket) = 0;
    
protected:
    bool serializeMessage(const cc::CommandMessage& commandMessage,
            zmq::socket_t& socket);
};

class PingCommand : public SocketCommand
{
public:
    PingCommand(QTimer& t, int64_t sn = 0);
    virtual ~PingCommand();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    bool commandHandler(zmq::socket_t& socket);
    
private:
    int64_t seqno;
    QTimer& timer;
};

class MoveCommand : public SocketCommand
{
protected:
    enum GroupDirection
    {
        forward,
        backward
    };
    
protected:
    MoveCommand(GroupDirection directionA, GroupDirection directionB,
            float power);
    
public:
    virtual ~MoveCommand();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    GroupDirection groupDirectionA;
    GroupDirection groupDirectionB;
    float drivePower;
};

class MoveForward : public MoveCommand
{
public:
    MoveForward(float power);
    virtual ~MoveForward();
};

class MoveBackward : public MoveCommand
{
public:
    MoveBackward(float power);
    virtual ~MoveBackward();
};

class RotateClockwise : public MoveCommand
{
public:
    RotateClockwise(float power);
    virtual ~RotateClockwise();
};

class RotateCounterClockwise : public MoveCommand
{
public:
    RotateCounterClockwise(float power);
    virtual ~RotateCounterClockwise();
};

#endif	/* COMMANDS_HPP */
