/* 
 * File:   load_util.hpp
 * Author: Ivan Gubochkin
 *
 * Created on August 21, 2014, 1:33 AM
 */

#ifndef LOAD_UTIL_HPP
#define	LOAD_UTIL_HPP

#include "mono_slam/mono_slam.hpp"

namespace test
{
    using namespace mslam;
    
    RealMatrix loadMatrix(const std::string& fileName)
    {
        std::ifstream stream(fileName, std::ifstream::binary);
        
        if (!stream)
        {
            SLAM_EXCEPTION(Exception, "Can't open file " + fileName);
        }
        
        int32_t r = -1, c = -1;
        stream.read((char *) &r, sizeof(int32_t));
        stream.read((char *) &c, sizeof(int32_t));

        cv::Mat1d m(r, c);
        stream.read((char *) m.data, r * c * sizeof(double));
        stream.close();
        
        return RealMatrix(m);
    }
}

#endif	/* LOAD_UTIL_HPP */
