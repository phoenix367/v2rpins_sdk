#include "Commands.hpp"
#include "messages/common.pb.h"

#include<QNetworkInterface>

namespace cc = cherokey::common;

uint64_t SocketCommand::indexCounter = 0;

SocketCommand::SocketCommand()
{
    commandIndex = indexCounter++;
}

SocketCommand::SocketCommand(const SocketCommand&)
{
    
}

SocketCommand::~SocketCommand()
{
    
}

bool SocketCommand::serializeMessage(const cc::CommandMessage& commandMessage,
        zmq::socket_t& socket)
{
    int messageSize = commandMessage.ByteSize();
    std::vector<uint8_t> outArray(messageSize);
    commandMessage.SerializeToArray(&outArray[0], messageSize);

    zmq::message_t outMessage(messageSize);
    memcpy(outMessage.data(), &outArray[0], messageSize);
    if (!socket.send(outMessage))
    {
        return false;
    }
    
    return true;
}

void SocketCommand::preInitCommand(cc::CommandMessage& commandMessage)
{
    commandMessage.set_cookie(0);
    commandMessage.set_command_index(getCommandIndex());
}

bool SocketCommand::handleReplyAck(zmq::socket_t& socket)
{
    zmq::message_t reply;
    
    zmq::pollitem_t items [] = { { socket, 0, ZMQ_POLLIN, 0 } };
    zmq::poll(items, 1, 1000);
    
    if (items [0].revents & ZMQ_POLLIN) 
    {
        if (!socket.recv(&reply))
        {
            return false;
        }

        cc::CommandReply cmdReply;
        if (!cmdReply.ParseFromArray(reply.data(), reply.size()))
        {
            return false;
        }

        if (cmdReply.type() != cc::CommandReply::ACK)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

uint64_t SocketCommand::getCommandIndex()
{
    return commandIndex;
}

PingCommand::PingCommand(QTimer& t, int64_t sn)
: seqno(sn)
, timer(t)
{
}

PingCommand::~PingCommand()
{
    
}

bool PingCommand::doCommand(zmq::socket_t& socket)
{
    bool result = commandHandler(socket);

    if (result)
    {
        timer.stop();
        timer.start(timer.interval());
    }
    
    return result;
}

bool PingCommand::commandHandler(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::PING);
    preInitCommand(commandMessage);

    cc::Ping* pingMsg = commandMessage.mutable_ping();
    pingMsg->set_seq_no(seqno);

    if (!serializeMessage(commandMessage, socket))
    {
        return false;
    }

    zmq::message_t reply;
    zmq::pollitem_t items [] = { { socket, 0, ZMQ_POLLIN, 0 } };
    zmq::poll(items, 1, 1000);
    
    if (items [0].revents & ZMQ_POLLIN) 
    {
        if (!socket.recv(&reply))
        {
            return false;
        }

        cc::CommandReply cmdReply;
        if (!cmdReply.ParseFromArray(reply.data(), reply.size()))
        {
            return false;
        }

        if (cmdReply.type() != cc::CommandReply::PONG)
        {
            return false;
        }

        cc::Pong *pongMsg = cmdReply.mutable_pong();
        if (pongMsg->seq_no() != seqno)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

CommandType PingCommand::getCommandType()
{
    return pingCommandType;
}

MoveCommand::MoveCommand(GroupDirection directionA, GroupDirection directionB,
        float power)
: groupDirectionA(directionA)
, groupDirectionB(directionB)
, drivePower(power)
{
    
}

MoveCommand::~MoveCommand()
{
    
}

bool MoveCommand::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    preInitCommand(commandMessage);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_move_action();
    cc::RunDriveGroup *groupA = moveCmd->mutable_run_group_a();
    
    switch (groupDirectionA)
    {
        case forward:
            groupA->set_direction(cc::RunDriveGroup::FORWARD);
            break;
        case backward:
            groupA->set_direction(cc::RunDriveGroup::BACKWARD);
            break;
    }
    
    groupA->set_power(drivePower);

    cc::RunDriveGroup *groupB = moveCmd->mutable_run_group_b();

    switch (groupDirectionB)
    {
        case forward:
            groupB->set_direction(cc::RunDriveGroup::FORWARD);
            break;
        case backward:
            groupB->set_direction(cc::RunDriveGroup::BACKWARD);
            break;
    }
    
    groupB->set_power(drivePower);

    if (!serializeMessage(commandMessage, socket))
    {
        return false;
    }

    bool replyResult = handleReplyAck(socket);

    return replyResult;
}

CommandType MoveCommand::getCommandType()
{
    return moveCommandType;
}

MoveForward::MoveForward(float power)
: MoveCommand(forward, forward, power)
{
    
}

MoveForward::~MoveForward()
{
    
}

MoveBackward::MoveBackward(float power)
: MoveCommand(backward, backward, power)
{
    
}

MoveBackward::~MoveBackward()
{
    
}

RotateClockwise::RotateClockwise(float power)
: MoveCommand(backward, forward, power)
{
    
}

RotateClockwise::~RotateClockwise()
{
    
}

RotateCounterClockwise::RotateCounterClockwise(float power)
: MoveCommand(forward, backward, power)
{
    
}

RotateCounterClockwise::~RotateCounterClockwise()
{
    
}

ShowVideoComposite::ShowVideoComposite(bool show, VideoType vt)
: showState(show)
, videoType(vt)
{
    
}

ShowVideoComposite::~ShowVideoComposite()
{
    
}

bool ShowVideoComposite::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::SHOW_VIDEO);
    preInitCommand(commandMessage);
    cc::ShowVideo* videoMsg = 
            commandMessage.mutable_show_video();
    videoMsg->set_show_state((showState) ? cc::ON : cc::OFF);
    switch (videoType)
    {
        case analog:
            videoMsg->set_channel_type(cc::RADIO);
            break;
        case digital:
            videoMsg->set_channel_type(cc::WIFI);
            videoMsg->set_receiver_address(getHostAddress());
            break;
    }

    if (!serializeMessage(commandMessage, socket))
    {
        return false;
    }

    bool replyResult = handleReplyAck(socket);

    return replyResult;
}

quint32 ShowVideoComposite::getHostAddress()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    
    for (int i = 0; i < list.size(); ++i) 
    {
        QNetworkInterface inter = list.at(i);
        QNetworkInterface::InterfaceFlags flags = inter.flags();
        if ((flags & QNetworkInterface::IsUp) &&
            !(flags & QNetworkInterface::IsLoopBack))
        {
            QList<QNetworkAddressEntry> addresses = inter.addressEntries();
            if (!addresses.empty())
            {
                return addresses.at(0).ip().toIPv4Address();
            }
        }
    }
    
    return 0;
}

CommandType ShowVideoComposite::getCommandType()
{
    return showVideoType;
}

SendSensorsInfo::SendSensorsInfo(bool send)
: sendState(send)
{
    
}

SendSensorsInfo::~SendSensorsInfo()
{
    
}

bool SendSensorsInfo::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::SENSORS_INFO);
    preInitCommand(commandMessage);
    cc::SendSensorsInfo* sensorsMsg = 
            commandMessage.mutable_sensors_send_state();
    sensorsMsg->set_send_state((sendState) ? cc::ON : cc::OFF);

    if (!serializeMessage(commandMessage, socket))
    {
        return false;
    }

    bool replyResult = handleReplyAck(socket);

    return replyResult;
}

CommandType SendSensorsInfo::getCommandType()
{
    return sendSensorsType;
}

RotateCommand::RotateCommand(float angle)
: rotateAngle(angle)
{
    
}

RotateCommand::~RotateCommand()
{
    
}

bool RotateCommand::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::ROTATION);
    preInitCommand(commandMessage);
    cc::RotationCommand* rotationCmd = 
            commandMessage.mutable_rotation_command();
    rotationCmd->set_rotation_angle(rotateAngle);

    if (!serializeMessage(commandMessage, socket))
    {
        return false;
    }

    bool replyResult = handleReplyAck(socket);

    return replyResult;
}

CommandType RotateCommand::getCommandType()
{
    return rotateCommandType;
}
