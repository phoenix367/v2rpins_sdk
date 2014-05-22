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
            RealMatrix Jnorm = normJac(x_k_k(cv::Range(3, 6)));
            int size_p_k_k = p_k_k.rows;
//            p_k_k = [   p_k_k(1:3,1:3)              p_k_k(1:3,4:7)*Jnorm'               p_k_k(1:3,8:size_p_k_k);
//                Jnorm*p_k_k(4:7,1:3)        Jnorm*p_k_k(4:7,4:7)*Jnorm'         Jnorm*p_k_k(4:7,8:size_p_k_k);
//                p_k_k(8:size_p_k_k,1:3)     p_k_k(8:size_p_k_k,4:7)*Jnorm'      p_k_k(8:size_p_k_k,8:size_p_k_k)];
//
//            x_k_k( 4:7 ) = x_k_k( 4:7 ) / norm( x_k_k( 4:7 ) );
        }
        else
        {
            x_k_k = x_km1_k;
            p_k_k = p_km1_k;
        }
    }
}
