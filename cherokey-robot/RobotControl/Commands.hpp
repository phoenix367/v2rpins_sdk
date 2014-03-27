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

enum CommandType
{
    pingCommandType,
    moveCommandType,
    showVideoType,
    sendSensorsType,
    rotateCommandType,
    notificationsCommandType
};

class SocketCommand
{
protected:
    SocketCommand();
    SocketCommand(const SocketCommand&);
    
public:
    virtual ~SocketCommand();
    
    virtual bool doCommand(zmq::socket_t& socket) = 0;
    virtual CommandType getCommandType() = 0;
    uint64_t getCommandIndex();
    
protected:
    bool serializeMessage(
            const cherokey::common::CommandMessage& commandMessage,
            zmq::socket_t& socket);
    void preInitCommand(cherokey::common::CommandMessage& commandMessage);
    bool handleReplyAck(zmq::socket_t& socket);
    
private:
    uint64_t commandIndex;
    static uint64_t indexCounter;
};

class PingCommand : public SocketCommand
{
public:
    PingCommand(QTimer& t, int64_t sn = 0);
    virtual ~PingCommand();
    
    virtual bool doCommand(zmq::socket_t& socket);
    virtual CommandType getCommandType();
    
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
    virtual CommandType getCommandType();
    
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
    virtual CommandType getCommandType();
    
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
    virtual CommandType getCommandType();
    
private:
    bool sendState;
};

class RotateCommand : public SocketCommand
{
public:
    RotateCommand(float angle);
    virtual ~RotateCommand();
    
    virtual bool doCommand(zmq::socket_t& socket);
    virtual CommandType getCommandType();
    
private:
    float rotateAngle;
};

class EnableNotifications : public SocketCommand
{
public:
    EnableNotifications(bool enable);
    virtual ~EnableNotifications();
    
    virtual bool doCommand(zmq::socket_t& socket);
    virtual CommandType getCommandType();
    
private:
    bool enableState;
};

#endif	/* COMMANDS_HPP */
