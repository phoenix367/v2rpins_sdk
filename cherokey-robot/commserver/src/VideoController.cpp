/* 
 * File:   VideoController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 26, 2013, 12:18 AM
 */

#include "VideoController.hpp"
#include "Exceptions.hpp"

#include <boost/system/error_code.hpp>
#include <signal.h>
#include <sys/wait.h>

std::unique_ptr<VideoController> VideoController::instance;

VideoController::VideoController() 
: videoTXPower(new pc::GPIOPin(pc::GPIO_PIN::gpio33, 
                pc::GPIO_DIRECTION::output))
, videoProcessPID(-1)
{
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
