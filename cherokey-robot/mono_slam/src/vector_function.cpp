#include "mono_slam/vector_function.hpp"
#include "mono_slam/exceptions.hpp"

namespace mslam
{
    RealMatrix operator -(const RealVector& a,
            const RealVector& b)
    {
        if (a.size() != b.size())
        {
            SLAM_EXCEPTION(IncorrectParamException, 
                    "Vector sizes doesn't equal");
        }
        
        RealMatrix r(a.size(), 1);
        for (size_t i = 0; i < r.rows; i++)
        {
            r(i) = a[i] - b[i];
        }
        
        return r;
    }
    
    RealVector operator +(const RealVector& a, const RealMatrix& b)
    {
        if (a.size() != b.rows || b.cols != 1)
        {
            SLAM_EXCEPTION(IncorrectParamException, 
                    "Invalid argument sizes");
        }
        
        RealVector r(a.size());
        for (size_t i = 0; i < r.size(); i++)
        {
            r[i] = a[i] + b(i, 0);
        }
        
        return r;
    }
    
    RealMatrix normJac(const RealVector& q)
    {
        if (q.size() < 4)
        {
            SLAM_EXCEPTION(IncorrectParamException, 
                    "Vector size is too small");
        }

        RealType r = q[0];
        RealType x = q[1];
        RealType y = q[2];
        RealType z = q[3];
        RealType f = pow(r * r + x * x + y * y + z * z, -3.0 / 2.0);

        RealMatrix J(4, 4);
        J(0, 0) = x * x + y * y + z * z;
        J(0, 1) = -r * x;
        J(0, 2) = -r * y;
        J(0, 3) = -r * z;
        J(1, 0) = -x * r;
        J(1, 1) = r * r + y * y + z * z;
        J(1, 2) = -x * y;
        J(1, 3) = -x * z;
        J(2, 0) = -y * r;
        J(2, 1) = -y * x;
        J(2, 2) = r * r + x * x + z * z;
        J(2, 3) = -y * z;
        J(3, 0) = -z * r;
        J(3, 1) = -z * x;
        J(3, 2) = -z * y;
        J(3, 3) = r * r + x * x + y * y;
        
        return f * J;
    }
    
    RealMatrix v2m(const RealVector& q)
    {
        RealMatrix r(q.size(), 1);
        std::copy(q.begin(), q.end(), r.begin());
        return r;
    }
    
    RealMatrix q2r(const RealVector& q)
    {
        if (q.size() < 4)
        {
            SLAM_EXCEPTION(IncorrectParamException, 
                    "Vector size is too small");
        }

        RealMatrix R(3, 3);
        RealType r = q[0];
        RealType x = q[1];
        RealType y = q[2];
        RealType z = q[3];

        R(0, 0) = r * r + x * x - y * y - z * z;
        R(0, 1) = 2 * (x * y - r * z);
        R(0, 2) = 2 * (z * x + r * y);
        R(1, 0) = 2 * (x * y + r * z);
        R(1, 1) = r * r - x * x + y * y - z * z;
        R(1, 2) = 2 * (y * z - r * x);
        R(2, 0) = 2 * (z * x - r * y);
        R(2, 1) = 2 * (y * z + r * x);
        R(2, 2) = r * r - x * x - y * y + z * z;
        
        return R;
    }
}
