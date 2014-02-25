/* 
 * File:   VideoController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 26, 2013, 12:18 AM
 */

#include "VideoController.hpp"
#include "Exceptions.hpp"
#include "ConfigManager.hpp"

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include <signal.h>
#include <sys/wait.h>

namespace ip = boost::asio::ip;

std::unique_ptr<VideoController> VideoController::instance;

VideoController::VideoController() 
: videoProcessPID(-1)
{
    auto instance = ConfigManager::getInstance();
    
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Configuration manager "
            "instance is null.");
    }
    
    auto pinConfig = instance->getPinsInfo();
    
    videoTXPower = std::unique_ptr<pc::GPIOPin>(
            new pc::GPIOPin(pinConfig.videoTxPowerPin, 
                pc::GPIO_DIRECTION::output));
}

VideoController::~VideoController() 
{
}

VideoController* VideoController::getInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<VideoController>(new VideoController());
    }
    
    return instance.get();
}

void VideoController::compositeVideo(bool showState)
{
    if (showState)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            COMM_EXCEPTION(LaunchException, "Failed to fork server process");
        }
        
        if (pid == 0)
        {
            int execResult = execlp(
                "/etc/cherokey-robot/run_video_composite", NULL);
            
            exit(execResult);
        }
        else
        {
            videoProcessPID = pid;
        }
        
        try
        {
            videoTXPower->setLogicalLevel(pc::GPIO_LOGIC_LEVEL::high);
        }
        catch (std::exception&)
        {
            stopChild();
            throw;
        }
    }
    else
    {
        stopChild();
        videoTXPower->setLogicalLevel(pc::GPIO_LOGIC_LEVEL::low);
    }
}

void VideoController::stopChild()
{
    if (videoProcessPID != -1)
    {
        kill(videoProcessPID, SIGINT);
        
        videoProcessPID = -1;
    }
}

void VideoController::digitalVideo(bool showState, uint32_t ipAddress)
{
    if (showState)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            COMM_EXCEPTION(LaunchException, "Failed to fork server process");
        }
        
        if (pid == 0)
        {
            ip::address_v4 addr(ipAddress);
            std::string str = addr.to_string();
            
            std::cout << "Send video to " << str << std::endl;
            int execResult = execlp(
                "/etc/cherokey-robot/run_video_wifi", "run_video_wifi",
                str.c_str(), NULL);
            
            exit(execResult);
        }
        else
        {
            videoProcessPID = pid;
        }
    }
    else
    {
        stopChild();
    }
}
