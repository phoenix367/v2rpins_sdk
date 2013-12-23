/* 
 * File:   Commands.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 23, 2013, 10:51 PM
 */

#ifndef COMMANDS_HPP
#define	COMMANDS_HPP

#include <zmq.hpp>
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
    PingCommand(int64_t sn = 0);
    virtual ~PingCommand();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    int64_t seqno;
};

class MoveForward : public SocketCommand
{
public:
    MoveForward(float power);
    virtual ~MoveForward();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    float drivePower;
};

class MoveBackward : public SocketCommand
{
public:
    MoveBackward(float power);
    virtual ~MoveBackward();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    float drivePower;
};

class RotateClockwise : public SocketCommand
{
public:
    RotateClockwise(float power);
    virtual ~RotateClockwise();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    float drivePower;
};

class RotateCounterClockwise : public SocketCommand
{
public:
    RotateCounterClockwise(float power);
    virtual ~RotateCounterClockwise();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    float drivePower;
};

#endif	/* COMMANDS_HPP */
