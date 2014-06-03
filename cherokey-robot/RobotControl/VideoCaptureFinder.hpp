/* 
 * File:   VideoCaptureFinder.hpp
 * Author: Ivan Gubochkin
 *
 * Created on June 4, 2014, 1:06 AM
 */

#ifndef VIDEOCAPTUREFINDER_HPP
#define	VIDEOCAPTUREFINDER_HPP

#include <vector>
#include <string>

class VideoCaptureFinder 
{
public:
    VideoCaptureFinder();
    virtual ~VideoCaptureFinder();
    
    std::vector<std::string> getCaptureDevices();
    
private:
    std::vector<std::string> captureDevices;
};

#endif	/* VIDEOCAPTUREFINDER_HPP */
