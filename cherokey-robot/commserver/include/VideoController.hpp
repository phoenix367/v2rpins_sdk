/* 
 * File:   VideoController.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 26, 2013, 12:18 AM
 */

#ifndef VIDEOCONTROLLER_HPP
#define	VIDEOCONTROLLER_HPP

#include <memory>
#include <boost/process.hpp>

#include "pincontroller/pincontroller.hpp"

class VideoController 
{
public:
    VideoController();
    virtual ~VideoController();
    
    static VideoController* getInstance();
    
    void compositeVideo(bool showState);
    
private:
    static std::unique_ptr<VideoController> instance;

    std::unique_ptr<pc::GPIOPin> videoTXPower;
    std::unique_ptr<boost::process::child> videoChild;
};

#endif	/* VIDEOCONTROLLER_HPP */
