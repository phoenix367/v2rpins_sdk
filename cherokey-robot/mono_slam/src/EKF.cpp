/* 
 * File:   EKF.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on May 18, 2014, 11:41 PM
 */

#include "mono_slam/EKF.hpp"
#include "mono_slam/vector_function.hpp"

namespace mslam
{
    EKF::EKF() 
    : std_a(0)
    , std_alpha(0)
    , std_z(0)
    , filterType(FilterType::constant_velocity)
    {
    }

    EKF::EKF(const RealVector& x, const RealMatrix& p, RealType a,
        RealType alpha, RealType z, FilterType type)
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
            RealMatrix p_k_k_new(size_p_k_k, size_p_k_k);
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

//            p_k_k = [   p_k_k(1:3,1:3)              p_k_k(1:3,4:7)*Jnorm'               p_k_k(1:3,8:size_p_k_k);
//                Jnorm*p_k_k(4:7,1:3)        Jnorm*p_k_k(4:7,4:7)*Jnorm'         Jnorm*p_k_k(4:7,8:size_p_k_k);
//                p_k_k(8:size_p_k_k,1:3)     p_k_k(8:size_p_k_k,4:7)*Jnorm'      p_k_k(8:size_p_k_k,8:size_p_k_k)];

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
}
