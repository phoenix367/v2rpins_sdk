/* 
 * File:   vector_function.hpp
 * Author: Ivan Gubochkin
 *
 * Created on May 19, 2014, 12:47 AM
 */

#ifndef MSLAM_VECTOR_FUNCTION_HPP
#define	MSLAM_VECTOR_FUNCTION_HPP

#include "declares.hpp"

namespace mslam
{
    RealMatrix operator -(const RealVector& a, const RealVector& b);
    RealVector operator +(const RealVector& a, const RealMatrix& b);
    RealVector operator +(const RealVector& a, const RealVector& b);
    RealVector operator *(const RealVector& a, const RealType& c);
    RealVector operator *(const RealType& c, const RealVector& a);
    RealVector operator /(const RealVector& a, const RealType& c);
    RealMatrix normJac(const RealVector& q);
    RealMatrix v2m(const RealVector& q);
    RealMatrix q2r(const RealVector& q);
    RealVector qprod(const RealVector& q, const RealVector& p);
    RealVector cross(const RealVector& a, const RealVector& b);
    RealType norm(const RealVector& v);
    RealVector v2q(const RealVector& v);
    RealVector qconj(const RealVector& q);
    
    template<typename T> T sum(const std::vector<T>& v)
    {
        T r = 0;
        for (size_t i = 0; i < v.size(); i++)
        {
            r += v[i];
        }
        
        return r;
    }
}

#endif	/* MSLAM_VECTOR_FUNCTION_HPP */
