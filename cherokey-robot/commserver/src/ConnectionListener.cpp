/* 
 * File:   ConnectionListener.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 18, 2013, 9:17 PM
 */

#include "ConnectionListener.hpp"
#include "Exceptions.hpp"
#include "DriveController.hpp"
#include "VideoController.hpp"
#include "common.pb.h"
#include "SensorsController.hpp"

namespace cc = cherokey::common;

extern zmq::context_t gContext;

ConnectionListener::ConnectionListener(
    std::shared_ptr<ConnectionInfo>& infoPtr) 
{
    if (infoPtr == nullptr)
    {
        COMM_EXCEPTION(NullPointerException, "Connection info is null.");
    }
    
    connectionParams = infoPtr;
    timer_create(CLOCK_REALTIME, NULL, &watchDogTimer);
}

ConnectionListener::~ConnectionListener() 
{
    timer_delete(watchDogTimer);
}

void ConnectionListener::run()
{
    std::ostringstream stream;
    
    stream << "tcp://" << connectionParams->ipAddress.to_string() << ":" <<
            connectionParams->port;

    zmq::socket_t socket(gContext, ZMQ_REP);
    
    std::cout << stream.str() << std::endl;
    
    socket.bind(stream.str().c_str());
    
    while (true)
    {
        zmq::message_t message;
        
        try
        {
            if (socket.recv(&message))
            {
                stopWatchdogTimer();
                startWatchDogTimer();
            }
        }
        catch (zmq::error_t& e)
        {
            if (e.num() == EINTR)
            {
                continue;
            }
            else
            {
                throw;
            }
        }
        
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
                case cc::CommandMessage::SHOW_VIDEO_COMPOSITE:
                    processVideoComposite(socket, commandMsg);
                    break;
                case cc::CommandMessage::SENSORS_INFO:
                    processSensrosInfo(socket, commandMsg);
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
    auto seqno = pingMsg.seq_no();

    std::cout << "Received ping message with seqno=" << seqno <<
            std::endl;
    cc::CommandReply replyMsg;
    replyMsg.set_type(cc::CommandReply::PONG);
    replyMsg.set_cookie(cookie);
    
    auto pongMsg = replyMsg.mutable_pong();
    pongMsg->set_seq_no(seqno);

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
        auto moveAction = msg.move_action();
        
        auto groupA = moveAction.run_group_a();
        runDriveGroup(groupA, DriveController::DriveGroup::GROUP_A);
        
        auto groupB = moveAction.run_group_b();
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

void ConnectionListener::processVideoComposite(zmq::socket_t& socket, 
        cherokey::common::CommandMessage& msg)
{
    auto cookie = msg.cookie();

    try
    {
        auto showAction = msg.mutable_show_video_composite();
        auto showState = showAction->show_state();
        
        auto videoInstance = VideoController::getInstance();
        if (!videoInstance)
        {
            COMM_EXCEPTION(NullPointerException, "Video controller instance "
                "is null.");
        }
        
        videoInstance->compositeVideo(showState == cc::ON);
        
        sendAck(cookie, socket);
    }
    catch (std::exception& e)
    {
        sendNack(e.what(), cookie, socket);
    }
}

void ConnectionListener::processSensrosInfo(zmq::socket_t& socket, 
            cherokey::common::CommandMessage& msg)
{
    auto cookie = msg.cookie();

    try
    {
        auto sendAction = msg.mutable_sensors_send_state();
        auto sendState = sendAction->send_state();
        
        auto sensorsInstance = SensorsController::getInstance();
        if (!sensorsInstance)
        {
            COMM_EXCEPTION(NullPointerException, "Sensors controller instance "
                "is null.");
        }
        
        auto configInstance = ConfigManager::getInstance();
        if (!configInstance)
        {
            COMM_EXCEPTION(NullPointerException, "Configuration manager instance "
                "is null.");
        }
        
        auto connectionInfo = configInstance->getSensorsConnectionInfo();
        if (!connectionInfo)
        {
            COMM_EXCEPTION(NullPointerException, "Connection information for "
                "sensors publisher is null.");
        }
        
        switch (sendState)
        {
            case cc::ON:
                sensorsInstance->startPublisher(*connectionInfo);
                break;
            case cc::OFF:
                sensorsInstance->stopPublisher();
                break;
        }
        
        sendAck(cookie, socket);
    }
    catch (std::exception& e)
    {
        sendNack(e.what(), cookie, socket);
    }
}

void ConnectionListener::startWatchDogTimer()
{
    struct itimerspec value;

    value.it_value.tv_sec = 3;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;

    timer_settime (watchDogTimer, 0, &value, NULL);
}

void ConnectionListener::stopWatchdogTimer()
{
    struct itimerspec value;

    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;

    timer_settime (watchDogTimer, 0, &value, NULL);
}

void ConnectionListener::onTimer(int sig)
{
    if (sig == SIGALRM)
    {
        auto sensorsInstance = SensorsController::getInstance();
        if (!sensorsInstance)
        {
            COMM_EXCEPTION(NullPointerException, "Sensors controller instance "
                "is null.");
        }
        
        sensorsInstance->stopPublisher();

        auto videoInstance = VideoController::getInstance();
        if (!videoInstance)
        {
            COMM_EXCEPTION(NullPointerException, "Video controller instance "
                "is null.");
        }
        
        videoInstance->compositeVideo(false);
    }
}
