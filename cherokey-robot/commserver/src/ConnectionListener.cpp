/* 
 * File:   ConnectionListener.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 18, 2013, 9:17 PM
 */

#include "ConnectionListener.hpp"
#include "Exceptions.hpp"
#include "DriveController.hpp"

namespace cc = cherokey::common;

ConnectionListener::ConnectionListener(
    std::shared_ptr<ConnectionInfo>& infoPtr) 
{
    if (infoPtr == nullptr)
    {
        COMM_EXCEPTION(NullPointerException, "Connection info is null.");
    }
    
    connectionParams = infoPtr;
}

ConnectionListener::~ConnectionListener() 
{
}

void ConnectionListener::run()
{
    std::ostringstream stream;
    
    stream << "tcp://" << connectionParams->ipAddress.to_string() << ":" <<
            connectionParams->port;

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    
    std::cout << stream.str() << std::endl;
    
    socket.bind(stream.str().c_str());
    
    while (true)
    {
        zmq::message_t message;
        socket.recv(&message);
        
        cc::CommandMessage commandMsg;
        if (!commandMsg.ParseFromArray(message.data(), message.size()))
        {
            std::cout << "Invalid message" << std::endl;
        }
        else
        {
            switch (commandMsg.type())
            {
                case cc::CommandMessage::PING:
                    processPing(socket, commandMsg);
                    break;
                case cc::CommandMessage::MOVE:
                    processMove(socket, commandMsg);
                    break;
                default:
                    std::cout << "Received unknown message with type " <<
                          commandMsg.type() << std::endl;  
            }
        }
    }
}

void ConnectionListener::processPing(zmq::socket_t& socket, 
        const cherokey::common::CommandMessage& msg)
{
    auto cookie = msg.cookie();
    auto pingMsg = msg.ping();
    auto seqno = pingMsg.seqno();

    std::cout << "Received ping message with seqno=" << seqno <<
            std::endl;
    cc::CommandReply replyMsg;
    replyMsg.set_type(cc::CommandReply::PONG);
    replyMsg.set_cookie(cookie);
    
    auto pongMsg = replyMsg.mutable_pong();
    pongMsg->set_seqno(seqno);

    int messageSize = replyMsg.ByteSize();
    std::vector<uint8_t> outArray(messageSize);
    replyMsg.SerializeToArray(&outArray[0], messageSize);

    socket.send(&outArray[0], messageSize);
    std::cout << "Sent pong message with seqno=" << seqno << std::endl;
}

void ConnectionListener::processMove(zmq::socket_t& socket, 
        const cherokey::common::CommandMessage& msg)
{
    auto cookie = msg.cookie();
    
    try
    {
        auto moveAction = msg.moveaction();
        
        auto groupA = moveAction.rungroupa();
        runDriveGroup(groupA, DriveController::DriveGroup::GROUP_A);
        
        auto groupB = moveAction.rungroupb();
        runDriveGroup(groupB, DriveController::DriveGroup::GROUP_B);

        sendAck(cookie, socket);
    }
    catch (std::exception& e)
    {
        sendNack(e.what(), cookie, socket);
    }
}

void ConnectionListener::sendNack(const std::string& reason, int64_t cookie,
        zmq::socket_t& socket)
{
    cc::CommandReply replyMsg;
    replyMsg.set_cookie(cookie);
    replyMsg.set_type(cc::CommandReply::NACK);

    auto nackMsg = replyMsg.mutable_nack();
    nackMsg->set_reason(reason);

    int messageSize = replyMsg.ByteSize();
    std::vector<uint8_t> outArray(messageSize);
    replyMsg.SerializeToArray(&outArray[0], messageSize);

    socket.send(&outArray[0], messageSize);
}

void ConnectionListener::sendAck(int64_t cookie, zmq::socket_t& socket)
{
    cc::CommandReply replyMsg;
    replyMsg.set_cookie(cookie);
    replyMsg.set_type(cc::CommandReply::ACK);

    int messageSize = replyMsg.ByteSize();
    std::vector<uint8_t> outArray(messageSize);
    replyMsg.SerializeToArray(&outArray[0], messageSize);

    socket.send(&outArray[0], messageSize);
}

void ConnectionListener::runDriveGroup(
    const cherokey::common::RunDriveGroup& group,
    DriveController::DriveGroup groupType)
{
    auto driveInstance = DriveController::getInstance();

    if (!driveInstance)
    {
        COMM_EXCEPTION(NullPointerException, "Drive controller instance "
                "is null.");
    }

    DriveController::MoveDirection direction;

    switch (group.direction())
    {
        case cc::RunDriveGroup::FORWARD:
            direction = DriveController::MoveDirection::MOVE_FORWARD;
            break;
        case cc::RunDriveGroup::BACKWARD:
            direction = DriveController::MoveDirection::MOVE_BACKWARD;
            break;
    }

    driveInstance->runDriveGroup(groupType, direction, group.power());
}
