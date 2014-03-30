/* 
 * File:   declares.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 20, 2014, 12:29 AM
 */

#ifndef DECLARES_HPP
#define	DECLARES_HPP

#include <opencv2/opencv.hpp>

namespace mslam
{
    typedef double RealType;
    typedef cv::Mat_<RealType> RealMatrix;
    
    struct CameraParams
    {
        RealType d;
        int nRows;
        int nCols;
        RealType Cx;
        RealType Cy;
        RealType k1;
        RealType k2;
        RealType f;
        cv::Matx<RealType, 3, 3> K;
    };
}

#endif	/* DECLARES_HPP */
