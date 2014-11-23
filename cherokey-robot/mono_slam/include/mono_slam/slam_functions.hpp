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
    RealMatrix44 dq3_by_dq1(const RealVector& q2_in);
    RealMatrix43 dqomegadt_by_domega(const RealVector& omega,
            RealType delta_t);
    RealMatrix func_Q(const RealVector& Xv, const RealMatrix& Pn,
            RealType delta_t, PredictionType type);
    RealMatrix44 dq3_by_dq2(const RealVector& q2_in);
    RealMatrix dfv_by_dxv(const RealVector& Xv, RealType delta_t, 
            PredictionType type);
    RealMatrix22 jacob_undistor_fm(const CameraParams& cam, 
            const RealMatrix21& uvd);
    RealMatrix21 undistor_fm(const CameraParams& cam, 
            const RealMatrix21& uvd);
    RealMatrix61 hinv(const CameraParams& cam, 
            const RealMatrix21& uvd,
            const RealVector& Xv,
            RealType initial_rho);
    RealMatrix23 dhu_dhrl(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi);
    RealMatrix22 dhd_dhu(const CameraParams& cam, const RealMatrix21& zi_d);
    RealMatrix23 dh_dhrl(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi);
    RealMatrix44 dqbar_by_dq();
    RealMatrix33 dR_by_dqz(const RealVector& q);
    RealMatrix33 dR_by_dqy(const RealVector& q);
    RealMatrix33 dR_by_dqx(const RealVector& q);
    RealMatrix33 dR_by_dq0(const RealVector& q);
    RealMatrix34 dRq_times_a_by_dq(const RealVector& q, 
            const RealMatrix31& aMat);
    RealMatrix34 dhrl_dqwr(const RealVector& Xv_km1_k,
            const RealVector& yi);
    RealMatrix24 dh_dqwr(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi);
    RealMatrix33 dhrl_drw(const RealVector& Xv_km1_k,
            const RealVector& yi);
    RealMatrix23 dh_drw(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi);
    RealMatrix dh_dxv(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi);
    RealMatrix36 dhrl_dy(const RealVector& Xv_km1_k,
            const RealVector& yi);
    RealMatrix26 dh_dy(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi);
    RealMatrix calculate_Hi_inverse_depth(const RealVector& Xv_km1_k,
            const RealVector& yi, const CameraParams& cam,
            int i, const std::vector<FeatureInfo>& features_info);
}

#endif	/* SLAM_FUNCTIONS_HPP */
