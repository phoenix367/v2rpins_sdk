/* 
 * File:   VideoController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 26, 2013, 12:18 AM
 */

#include "VideoController.hpp"
#include "Exceptions.hpp"

#include <boost/system/error_code.hpp>

namespace bp = ::boost::process;

std::unique_ptr<VideoController> VideoController::instance;

VideoController::VideoController() 
: videoTXPower(new pc::GPIOPin(pc::GPIO_PIN::gpio33, 
                pc::GPIO_DIRECTION::output))
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
        std::string exec = "/etc/cherokey-robot/run_video_composite";
        
        boost::system::error_code ec;
        std::vector<std::string> args;
        bp::context ctx;
        
    ctx.stdin_behavior = bp::inherit_stream(); 
    ctx.stdout_behavior = bp::inherit_stream(); 
    ctx.stderr_behavior = bp::inherit_stream(); 

        videoChild = std::unique_ptr<bp::child>(
                new bp::child(boost::process::launch_shell(exec, ctx)));
        std::cout << "GStreamer PID = " << videoChild->get_id() << std::endl;
        
        try
        {
            videoTXPower->setLogicalLevel(pc::GPIO_LOGIC_LEVEL::high);
        }
        catch (std::exception& e)
        {
            videoChild->terminate();
            throw;
        }
    }
    else
    {
        if (videoChild)
        {
            std::cout << "Terminate gstreamer" << std::endl;
            
            try
            {
                videoChild->terminate(true);
            }
            catch (std::exception& e)
            {
                std::cout << e.what() << std::endl;
                throw;
            }
        }
        
        videoTXPower->setLogicalLevel(pc::GPIO_LOGIC_LEVEL::low);
    }
}
