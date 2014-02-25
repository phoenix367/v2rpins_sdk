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

namespace cherokey
{
    namespace common
    {
        class CommandMessage;
    }
}

class SocketCommand
{
public:
    virtual ~SocketCommand();
    
    virtual bool doCommand(zmq::socket_t& socket) = 0;
    
protected:
    bool serializeMessage(
            const cherokey::common::CommandMessage& commandMessage,
            zmq::socket_t& socket);
    bool handleReplyAck(zmq::socket_t& socket);
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

class ShowVideoComposite : public SocketCommand
{
public:
    enum VideoType
    {
        analog,
        digital
    };
    
public:
    ShowVideoComposite(bool show, VideoType vt);
    virtual ~ShowVideoComposite();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    quint32 getHostAddress();
    
private:
    bool showState;
    VideoType videoType;
};

class SendSensorsInfo : public SocketCommand
{
public:
    SendSensorsInfo(bool send);
    virtual ~SendSensorsInfo();
    
    virtual bool doCommand(zmq::socket_t& socket);
    
private:
    bool sendState;
};

#endif	/* COMMANDS_HPP */
