/* 
 * File:   slam_functions.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 19, 2014, 11:52 PM
 */

#ifndef SLAM_FUNCTIONS_HPP
#define	SLAM_FUNCTIONS_HPP

#include "declares.hpp"
#include "EKF.hpp"

namespace mslam
{
    void randomMatrix6D(int nPointsRand, RealMatrix& rndMat);
    RealMatrix hu(const RealMatrix31& yi, const CameraParams& cam);
    RealMatrix distort_fm(const RealMatrix& uv, const CameraParams& cam);
    RealMatrix31 m(RealType a, RealType b);
    RealMatrix hi_inverse_depth(const RealMatrix61& yinit,
            const RealMatrix31& t_wc,
            const RealMatrix33& r_wc,
            const CameraParams& cam);
    void predict_camera_measurements(const RealVector& x_k_k, 
            const CameraParams& cam,
            std::vector<FeatureInfo>& features_info);
    void initialize_a_feature(int step, const CameraParams& cam, 
            const cv::Mat1b& im_k, 
            std::vector<FeatureInfo>& features_info,
            EKF& filter, RealMatrix& uv);
    void find_ransac_features(const RealMatrix21& search_region_center,
            const cv::Mat1b& im_k, const CameraParams& cam,
            int initializing_box_semisize[2]);
    RealVector fv(const RealVector& X_k_k, RealType delta, 
            PredictionType type);
}

#endif	/* SLAM_FUNCTIONS_HPP */
