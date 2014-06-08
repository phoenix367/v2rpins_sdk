/* 
 * File:   VideoCaptureFinder.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on June 4, 2014, 1:06 AM
 */

#include "VideoCaptureFinder.hpp"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

namespace fs = boost::filesystem;

VideoCaptureFinder::VideoCaptureFinder() 
{
    fs::path someDir("/dev");
    fs::directory_iterator end_iter;
    const boost::regex videoFilter( "/dev/video.*" );


//    typedef std::multimap<std::time_t, fs::path> result_set_t;
//    result_set_t result_set;

    if ( fs::exists(someDir) && fs::is_directory(someDir))
    {
        for (fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter; 
                ++dir_iter)
        {
            if (fs::is_other(dir_iter->status()) )
            {
                boost::smatch what;
                std::string pathStr = dir_iter->path().string();
                
                // Skip if no match
                if ( !boost::regex_match( pathStr, 
                        what, videoFilter ) ) 
                {
                    continue;
                }
                               
                int deviceHandle = open (pathStr.c_str(), O_RDWR);
                if (deviceHandle == -1)
                {       // could not open device
                    continue;
                }              

                v4l2_capability capability;
                if (ioctl (deviceHandle, VIDIOC_QUERYCAP, &capability) != -1)
                {       // query was successful
                    std::string cardName((char *) capability.card);
                    if (cardName == "usbtv")
                    {
                        v4l2_frmsizeenum sizes;
                        memset(&sizes, 0, sizeof(v4l2_frmsizeenum));
                        if (ioctl(deviceHandle, VIDIOC_ENUM_FRAMESIZES, &sizes)  != -1)
                        {
                            
                        }
                        captureDevices.push_back(pathStr);
                    }
                }
                else
                {       // query failed
                    std::cout << "Can't get video device capabilities" <<
                            std::endl;
                }

                close(deviceHandle);
            }
        }
    }
}

VideoCaptureFinder::~VideoCaptureFinder() 
{
}

std::vector<std::string> VideoCaptureFinder::getCaptureDevices()
{
    return captureDevices;
}
