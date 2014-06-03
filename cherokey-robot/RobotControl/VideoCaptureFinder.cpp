/* 
 * File:   VideoCaptureFinder.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on June 4, 2014, 1:06 AM
 */

#include "VideoCaptureFinder.hpp"

#include <boost/filesystem.hpp>
#include <fcntl.h>

namespace fs = boost::filesystem;

VideoCaptureFinder::VideoCaptureFinder() 
{
    fs::path someDir("/dev/video*");
    fs::directory_iterator end_iter;

//    typedef std::multimap<std::time_t, fs::path> result_set_t;
//    result_set_t result_set;

    if ( fs::exists(someDir) && fs::is_directory(someDir))
    {
        for (fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter; 
                ++dir_iter)
        {
            if (fs::is_regular_file(dir_iter->status()) )
            {
//                int deviceHandle = open (dir_iter->path(), O_RDWR);
//                if (deviceHandle == -1)
//                {       // could not open device
//                    continue;
//                }
//
//                result_set.insert(result_set_t::value_type(
//                    fs::last_write_time(dir_iter->status()), *dir_iter);
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
