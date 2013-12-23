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

PingCommand::PingCommand(int64_t sn)
: seqno(sn)
{
    
}

PingCommand::~PingCommand()
{
    
}

bool PingCommand::doCommand(zmq::socket_t& socket)
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
    
    return true;
}

MoveForward::MoveForward(float power)
: drivePower(power)
{
    
}

MoveForward::~MoveForward()
{
    
}

bool MoveForward::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_moveaction();
    cc::RunDriveGroup *groupA = moveCmd->mutable_rungroupa();
    groupA->set_direction(cc::RunDriveGroup::FORWARD);
    groupA->set_power(drivePower);

    cc::RunDriveGroup *groupB = moveCmd->mutable_rungroupb();
    groupB->set_direction(cc::RunDriveGroup::FORWARD);
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

    return true;
}

MoveBackward::MoveBackward(float power)
: drivePower(power)
{
    
}

MoveBackward::~MoveBackward()
{
    
}

bool MoveBackward::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_moveaction();
    cc::RunDriveGroup *groupA = moveCmd->mutable_rungroupa();
    groupA->set_direction(cc::RunDriveGroup::BACKWARD);
    groupA->set_power(drivePower);

    cc::RunDriveGroup *groupB = moveCmd->mutable_rungroupb();
    groupB->set_direction(cc::RunDriveGroup::BACKWARD);
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

    return true;
}

RotateClockwise::RotateClockwise(float power)
: drivePower(power)
{
    
}

RotateClockwise::~RotateClockwise()
{
    
}

bool RotateClockwise::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_moveaction();
    cc::RunDriveGroup *groupA = moveCmd->mutable_rungroupa();
    groupA->set_direction(cc::RunDriveGroup::BACKWARD);
    groupA->set_power(drivePower);

    cc::RunDriveGroup *groupB = moveCmd->mutable_rungroupb();
    groupB->set_direction(cc::RunDriveGroup::FORWARD);
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

    return true;
}

RotateCounterClockwise::RotateCounterClockwise(float power)
: drivePower(power)
{
    
}

RotateCounterClockwise::~RotateCounterClockwise()
{
    
}

bool RotateCounterClockwise::doCommand(zmq::socket_t& socket)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_moveaction();
    cc::RunDriveGroup *groupA = moveCmd->mutable_rungroupa();
    groupA->set_direction(cc::RunDriveGroup::FORWARD);
    groupA->set_power(drivePower);

    cc::RunDriveGroup *groupB = moveCmd->mutable_rungroupb();
    groupB->set_direction(cc::RunDriveGroup::BACKWARD);
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

    return true;
}
