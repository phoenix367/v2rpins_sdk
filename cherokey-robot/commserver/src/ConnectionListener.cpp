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
#include "IndicatorController.hpp"
#include "PIDController.hpp"

namespace cc = cherokey::common;

extern zmq::context_t gContext;

const std::string ConnectionListener::INTERNAL_COMMAND_ADDR =
    "inproc://commands";

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

    zmq::socket_t socketCmd(gContext, ZMQ_REP), 
            socketInt(gContext, ZMQ_PULL);
    
    std::cout << stream.str() << std::endl;
    
    socketCmd.bind(stream.str().c_str());
    socketInt.bind(INTERNAL_COMMAND_ADDR.c_str());
    
    while (true)
    {
        zmq::message_t message;
        
        zmq::pollitem_t items [] = { { socketCmd, 0, ZMQ_POLLIN, 0 },
            { socketInt, 0, ZMQ_POLLIN, 0 } };
        try
        {
            zmq::poll(items, 2, 1000);
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

        if (items[0].revents & ZMQ_POLLIN) 
        {
            if (socketCmd.recv(&message))
            {
                stopWatchdogTimer();
                startWatchDogTimer();
            }
        
            cc::CommandMessage commandMsg;
            if (!commandMsg.ParseFromArray(message.data(), message.size()))
            {
                std::cout << "Invalid command message" << std::endl;
            }
            else
            {
                switch (commandMsg.type())
                {
                    case cc::CommandMessage::PING:
                        processPing(socketCmd, commandMsg);
                        break;
                    case cc::CommandMessage::MOVE:
                        processMove(socketCmd, commandMsg);
                        break;
                    case cc::CommandMessage::SHOW_VIDEO:
                        processVideo(socketCmd, commandMsg);
                        break;
                    case cc::CommandMessage::SENSORS_INFO:
                        processSensrosInfo(socketCmd, commandMsg);
                        break;
                    case cc::CommandMessage::ROTATION:
                        processRotation(socketCmd, commandMsg);
                        break;
                    default:
                        std::cout << "Received unknown message with type " <<
                              commandMsg.type() << std::endl;  
                }
            }
        }
        
        if (items[1].revents & ZMQ_POLLIN) 
        {
            if (socketInt.recv(&message))
            {
                cc::CommandMessage commandMsg;
                if (!commandMsg.ParseFromArray(message.data(), 
                        message.size()))
                {
                    std::cout << "Invalid internal message" << std::endl;
                }
                else
                {
                    switch (commandMsg.type())
                    {
                        case cc::CommandMessage::CALIBRATION_STATE:
                            processCalibration(commandMsg);
                            break;
                        case cc::CommandMessage::MOVE:
                            processMoveInternal(commandMsg);
                            break;
                        default:
                            std::cout << 
                                  "Received unknown internal message with type " <<
                                  commandMsg.type() << std::endl;  
                    }
                }
            }
        }
    }
    
    socketCmd.close();
    socketInt.close();
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

void ConnectionListener::processVideo(zmq::socket_t& socket, 
        cherokey::common::CommandMessage& msg)
{
    auto cookie = msg.cookie();
    
    try
    {
        auto videoMsg = msg.mutable_show_video();
        auto showState = videoMsg->show_state();
        
        auto videoInstance = VideoController::getInstance();
        if (!videoInstance)
        {
            COMM_EXCEPTION(NullPointerException, "Video controller instance "
                "is null.");
        }
        
        switch (videoMsg->channel_type())
        {
            case cc::WIFI:
                {
                    uint32_t ipAddress = videoMsg->receiver_address();
                    videoInstance->digitalVideo(showState == cc::ON,
                            ipAddress);
                }
                break;
            case cc::RADIO:
                videoInstance->compositeVideo(showState == cc::ON);
                break;
        }
        
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

void ConnectionListener::processCalibration(
        cherokey::common::CommandMessage& msg)
{
    try
    {
        auto instance = IndicatorController::getInstance();
        
        if (!instance)
        {
            COMM_EXCEPTION(NullPointerException,
                    "Indicator controller instance is null");
        }
        
        auto stateMsg = msg.mutable_calibration_state();
        bool calibrationPhase = (stateMsg->state() == cc::ON);
        instance->showCalibrationState(calibrationPhase);
        
        if (!calibrationPhase)
        {
            auto pidInstance = PIDController::getInstance();
            
            if (pidInstance)
            {
                pidInstance->startController();
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception occured: " << e.what() << std::endl;
    }
}

void ConnectionListener::processMoveInternal(
        cherokey::common::CommandMessage& msg)
{
    try
    {
        auto moveAction = msg.move_action();
        
        auto groupA = moveAction.run_group_a();
        runDriveGroup(groupA, DriveController::DriveGroup::GROUP_A);
        
        auto groupB = moveAction.run_group_b();
        runDriveGroup(groupB, DriveController::DriveGroup::GROUP_B);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception occured: " << e.what() << std::endl;
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

        auto pidInstance = PIDController::getInstance();

        if (pidInstance)
        {
            pidInstance->stopController();
        }

        auto driveInstance = DriveController::getInstance();

        if (!driveInstance)
        {
            COMM_EXCEPTION(NullPointerException, "Drive controller instance "
                    "is null.");
        }
        
        driveInstance->stopDrives();
    }
}

void ConnectionListener::processRotation(zmq::socket_t& socket, 
        const cherokey::common::CommandMessage& msg)
{
    auto cookie = msg.cookie();

    try
    {
        auto rotationAction = msg.rotation_command();
        float angle = rotationAction.rotation_angle();
        
        auto instance = PIDController::getInstance();
        if (!instance)
        {
            COMM_EXCEPTION(NullPointerException, "PID controller instance "
                    "is null");
        }
        
        instance->putRotation(angle);
        sendAck(cookie, socket);
    }
    catch (std::exception& e)
    {
        sendNack(e.what(), cookie, socket);
    }
}
