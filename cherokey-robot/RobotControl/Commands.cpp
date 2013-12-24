#include "Commands.hpp"

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
    commandMessage.set_cookie(0);
    cc::Ping* pingMsg = commandMessage.mutable_ping();
    pingMsg->set_seqno(seqno);

    if (!serializeMessage(commandMessage, socket))
    {
        return false;
    }

    zmq::message_t reply;
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
    if (pongMsg->seqno() != seqno)
    {
        return false;
    }

    return true;
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
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_moveaction();
    cc::RunDriveGroup *groupA = moveCmd->mutable_rungroupa();
    
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

    cc::RunDriveGroup *groupB = moveCmd->mutable_rungroupb();

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

    zmq::message_t reply;
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

    return true;
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
