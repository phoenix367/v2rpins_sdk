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
    RealMatrix normJac(const RealVector& q);
    RealMatrix v2m(const RealVector& q);
    RealMatrix q2r(const RealVector& q);
}

#endif	/* MSLAM_VECTOR_FUNCTION_HPP */
