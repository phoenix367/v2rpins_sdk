/* 
 * File:   declares.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 20, 2014, 12:29 AM
 */

#ifndef MSLAM_DECLARES_HPP
#define	MSLAM_DECLARES_HPP

#include <opencv2/opencv.hpp>

namespace mslam
{
    typedef double RealType;
    typedef cv::Mat_<RealType> RealMatrix;
    typedef cv::Vector<RealType> RealVector;
    typedef cv::Matx<RealType, 3, 1> RealMatrix31;
    typedef cv::Matx<RealType, 3, 2> RealMatrix32;
    typedef cv::Matx<RealType, 6, 1> RealMatrix61;
    typedef cv::Matx<RealType, 3, 3> RealMatrix33;
    typedef cv::Matx<RealType, 3, 4> RealMatrix34;
    typedef cv::Matx<RealType, 3, 6> RealMatrix36;
    typedef cv::Matx<RealType, 2, 1> RealMatrix21;
    typedef cv::Matx<RealType, 2, 2> RealMatrix22;
    typedef cv::Matx<RealType, 2, 3> RealMatrix23;
    typedef cv::Matx<RealType, 2, 4> RealMatrix24;
    typedef cv::Matx<RealType, 2, 6> RealMatrix26;
    typedef cv::Matx<RealType, 4, 4> RealMatrix44;
    typedef cv::Matx<RealType, 4, 3> RealMatrix43;
    
    enum FeatureType
    {
        cartesian,
        inversedepth
    };
    
    enum PredictionType
    {
        constant_orientation,
        constant_position,
        constant_position_and_orientation,
        constant_position_and_orientation_location_noise,
        constant_velocity
    };
    
    struct CameraParams
    {
        RealType dx;
        RealType dy;
        int nRows;
        int nCols;
        RealType Cx;
        RealType Cy;
        RealType k1;
        RealType k2;
        RealType f;
        cv::Matx<RealType, 3, 3> K;
    };
    
    struct FeatureInfo
    {
        cv::Mat1b patch_when_initialized;
        RealMatrix patch_when_matching;
        RealMatrix31 r_wc_when_initialized;
        RealMatrix33 R_wc_when_initialized;
        RealMatrix21 uv_when_initialized;
        int half_patch_size_when_initialized;
        int half_patch_size_when_matching;
        int times_predicted;
        int times_measured;
        int init_frame;
        RealMatrix21 init_measurement;
        FeatureType type;
        RealMatrix61 yi;
        bool individually_compatible;
        int low_innovation_inlier;
        int high_innovation_inlier;
        RealMatrix21 z;
        RealMatrix h;
        RealMatrix H;
        RealMatrix22 S;
        int state_size;
        int measurement_size;
        RealMatrix22 R;
    };
}

#endif	/* MSLAM_DECLARES_HPP */
