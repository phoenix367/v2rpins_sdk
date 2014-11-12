/* 
 * File:   EKF.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on May 18, 2014, 11:41 PM
 */

#include <algorithm>

#include "mono_slam/EKF.hpp"
#include "mono_slam/vector_function.hpp"
#include "mono_slam/slam_functions.hpp"

namespace mslam
{
    EKF::EKF() 
    : std_a(0)
    , std_alpha(0)
    , std_z(0)
    , filterType(constant_velocity)
    {
    }

    EKF::EKF(const RealVector& x, const RealMatrix& p, RealType a,
        RealType alpha, RealType z, PredictionType type)
    : x_k_k(x)
    , p_k_k(p)
    , std_a(a)
    , std_alpha(alpha)
    , std_z(z)
    , filterType(type)
    {
        
    }
    
    EKF::~EKF() 
    {
    }

    void EKF::update(const RealVector& x_km1_k, const RealMatrix& p_km1_k, 
        const RealMatrix& H, const RealMatrix& R, const RealVector& z, 
        const RealVector& h)
    {
        if (!z.empty())
        {
            // filter gain
            RealMatrix S = H * p_km1_k * H.t() + R;
            RealMatrix invS;
            
            cv::invert(S, invS);
            RealMatrix K = p_km1_k * H.t() * invS;
            

            // updated state and covariance
            x_k_k = x_km1_k + K * (z - h);
            p_k_k = p_km1_k - K * S * K.t();
            p_k_k = 0.5 * p_k_k + 0.5 * p_k_k.t();

            // normalize the quaternion
            RealMatrix Jnorm = normJac(x_k_k(cv::Range(3, 7)));
            
            // update P matrix
            int size_p_k_k = p_k_k.rows;
            RealMatrix p_k_k_new = RealMatrix::zeros(size_p_k_k, size_p_k_k);
            p_k_k(cv::Range(0, 3), cv::Range(0, 3)).copyTo(
                    p_k_k_new(cv::Range(0, 3), cv::Range(0, 3)));
            RealMatrix tmp = p_k_k(cv::Range(0, 3), cv::Range(3, 7)) * 
                    Jnorm.t();
            tmp.copyTo(p_k_k_new(cv::Range(0, 3), cv::Range(3, 7)));
            p_k_k(cv::Range(0, 3), cv::Range(7, size_p_k_k)).copyTo(
                    p_k_k_new(cv::Range(0, 3), cv::Range(7, size_p_k_k)));
            
            tmp = Jnorm * p_k_k(cv::Range(3, 7), cv::Range(0, 3));
            tmp.copyTo(p_k_k_new(cv::Range(3, 7), cv::Range(0, 3)));
            
            tmp = Jnorm * p_k_k(cv::Range(3, 7), cv::Range(3, 7)) * Jnorm.t();
            tmp.copyTo(p_k_k_new(cv::Range(3, 7), cv::Range(3, 7)));
            
            tmp = Jnorm * p_k_k(cv::Range(3, 7), cv::Range(7, size_p_k_k));
            tmp.copyTo(p_k_k_new(cv::Range(3, 7), cv::Range(7, size_p_k_k)));

            p_k_k(cv::Range(7, size_p_k_k), cv::Range(0, 3)).copyTo(
                    p_k_k_new(cv::Range(7, size_p_k_k), cv::Range(0, 3)));
            
            tmp = p_k_k(cv::Range(7, size_p_k_k), cv::Range(3, 7)) * Jnorm.t();
            tmp.copyTo(p_k_k_new(cv::Range(7, size_p_k_k), cv::Range(3, 7)));

            p_k_k(cv::Range(7, size_p_k_k), cv::Range(7, size_p_k_k)).copyTo(
                    p_k_k_new(cv::Range(7, size_p_k_k), 
                    cv::Range(7, size_p_k_k)));
            p_k_k = p_k_k_new;

            RealType n = (RealType) cv::norm(v2m(x_k_k( cv::Range(3, 7) )));
            for (size_t i = 3; i < 7; i++)
            {
                x_k_k[i] /= n;
            }
        }
        else
        {
            x_k_k = x_km1_k;
            p_k_k = p_km1_k;
        }
    }

    RealVector EKF::getX()
    {
        return x_k_k;
    }
    
    RealMatrix EKF::getP()
    {
        return p_k_k;
    }
    
    RealType EKF::getStdZ()
    {
        return std_z;
    }
    
    void EKF::predict()
    {
        RealType delta_t = 1;
        RealVector Xv_km1_k = fv(x_k_k(cv::Range(0, 13)), delta_t, 
                filterType);
        x_k_km1.resize(x_k_k.size());
        std::copy(Xv_km1_k.begin(), Xv_km1_k.end(), x_k_km1.begin());
        std::copy(x_k_k.begin() + 13, x_k_k.end(), x_k_km1.begin() + 13);
        
        RealMatrix F = dfv_by_dxv(x_k_k(cv::Range(0, 13)), delta_t,
                filterType);
        
        mslam::RealType linear_acceleration_noise_covariance = 
                std_a * delta_t;
        linear_acceleration_noise_covariance *= 
                linear_acceleration_noise_covariance;
        mslam::RealType angular_acceleration_noise_covariance = 
                std_alpha * delta_t;
        angular_acceleration_noise_covariance *=
                angular_acceleration_noise_covariance;
        mslam::RealMatrix Pn = mslam::RealMatrix::zeros(6, 6);

        Pn(0, 0) = linear_acceleration_noise_covariance;
        Pn(1, 1) = linear_acceleration_noise_covariance;
        Pn(2, 2) = linear_acceleration_noise_covariance;
        Pn(3, 3) = angular_acceleration_noise_covariance;
        Pn(4, 4) = angular_acceleration_noise_covariance;
        Pn(5, 5) = angular_acceleration_noise_covariance;

        RealMatrix Q = func_Q(x_k_k(cv::Range(0, 13)), Pn, delta_t, 
                filterType);
        
        RealMatrix A = F * p_k_k(cv::Range(0, 13), cv::Range(0, 13)) * F.t() + 
                Q;
        RealMatrix B = F * p_k_k(cv::Range(0, 13), cv::Range(13, p_k_k.rows));
        RealMatrix C = p_k_k(cv::Range(13, p_k_k.rows), cv::Range(0, 13)) *
                F.t();
        RealMatrix D = p_k_k(cv::Range(13, p_k_k.rows),
                cv::Range(13, p_k_k.rows));
        
        p_k_km1 = RealMatrix::zeros(A.rows + C.rows, A.cols + B.cols);
        A.copyTo(p_k_km1(cv::Range(0, A.rows), cv::Range(0, A.cols)));
        B.copyTo(p_k_km1(cv::Range(0, B.rows), cv::Range(A.cols, 
                p_k_km1.cols)));
        C.copyTo(p_k_km1(cv::Range(A.rows, p_k_km1.rows), 
                cv::Range(0, C.cols)));
        D.copyTo(p_k_km1(cv::Range(B.rows, p_k_km1.rows), cv::Range(A.cols, 
                p_k_km1.cols)));
    }

    RealVector EKF::getXM1()
    {
        return x_k_km1;
    }
    
    RealMatrix EKF::getPM1()
    {
        return p_k_km1;
    }
}
