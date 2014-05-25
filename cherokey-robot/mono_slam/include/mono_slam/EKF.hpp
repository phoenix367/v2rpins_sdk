/* 
 * File:   EKF.hpp
 * Author: Ivan Gubochkin
 *
 * Created on May 18, 2014, 11:41 PM
 */

#ifndef EKF_HPP
#define	EKF_HPP

#include "declares.hpp"

namespace mslam
{
    class EKF 
    {
    public:
        enum class FilterType
        {
            constant_velocity
        };
        
    public:
        EKF();
        EKF(const RealVector& x, const RealMatrix& p, RealType a,
            RealType alpha, RealType z, FilterType type);
        virtual ~EKF();
        
        void update(const RealVector& x_km1_k, const RealMatrix& p_km1_k, 
            const RealMatrix& H, const RealMatrix& R, const RealVector& z, 
            const RealVector& h);
        
        RealVector getX();
        RealMatrix getP();
        
    private:
        RealVector x_k_k;
        RealMatrix p_k_k;
        RealType std_a;
        RealType std_alpha;
        RealType std_z;
        FilterType filterType;
        RealVector x_k_k_mixing_estimate;
        RealMatrix p_k_k_mixing_covariance;
    };
}

#endif	/* EKF_HPP */
