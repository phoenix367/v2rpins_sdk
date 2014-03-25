/* 
 * File:   ConnectionListener.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 18, 2013, 9:17 PM
 */

#ifndef CONNECTIONLISTENER_HPP
#define	CONNECTIONLISTENER_HPP

#include "ConfigManager.hpp"
#include "DriveController.hpp"
#include <zmq.hpp>

namespace cherokey
{
    namespace common
    {
        class CommandMessage;
        class RunDriveGroup;
    }
}

class ConnectionListener 
{
public:
    static const std::string INTERNAL_COMMAND_ADDR;
    static const std::string INTERNAL_NOTIFY_ADDR;

public:
    ConnectionListener(std::shared_ptr<ConnectionInfo>& infoPtr);
    virtual ~ConnectionListener();
    
    void run();
    
private:
    void processPing(zmq::socket_t& socket, 
            const cherokey::common::CommandMessage& msg);
    void processMove(zmq::socket_t& socket, 
            const cherokey::common::CommandMessage& msg);
    void sendNack(const std::string& reason, int64_t cookie,
            zmq::socket_t& socket);
    void sendAck(int64_t cookie, zmq::socket_t& socket);
    void runDriveGroup(const cherokey::common::RunDriveGroup& group,
            DriveController::DriveGroup groupType);
    void processVideo(zmq::socket_t& socket, 
            cherokey::common::CommandMessage& msg);
    void processSensorsInfo(zmq::socket_t& socket, 
            cherokey::common::CommandMessage& msg);
    void processCalibration(
            cherokey::common::CommandMessage& msg);
    void processMoveInternal(
            cherokey::common::CommandMessage& msg);
    void processRotation(zmq::socket_t& socket, 
            const cherokey::common::CommandMessage& msg);
    
    void startWatchDogTimer();
    void stopWatchdogTimer();
    void connectNotificator(const ConnectionInfo& info);
    void disconnectNotificator();
    
public:
    void onTimer(int sig);
    
private:
    std::shared_ptr<ConnectionInfo> connectionParams;
    timer_t watchDogTimer;
    zmq::socket_t notificationSocket;
};

#endif	/* CONNECTIONLISTENER_HPP */
