#include "mono_slam/slam_functions.hpp"

namespace mslam
{
    void randomMatrix6D(int nPointsRand, RealMatrix& rndMat)
    {
        cv::RNG rng = cv::theRNG();
        RealMatrix X(1, nPointsRand);
        RealMatrix Y(1, nPointsRand);
        RealMatrix Z(1, nPointsRand);
        RealMatrix theta(1, nPointsRand);
        RealMatrix phi(1, nPointsRand);
        RealMatrix lambda(1, nPointsRand);
        
        rng.fill(X, cv::RNG::UNIFORM, 1, -0.5);
        rng.fill(Y, cv::RNG::UNIFORM, 1, -0.5);
        rng.fill(Z, cv::RNG::UNIFORM, 1, -0.5);
        rng.fill(theta, cv::RNG::UNIFORM, 1, -0.5);
        rng.fill(phi, cv::RNG::UNIFORM, 1, -0.5);
        rng.fill(lambda, cv::RNG::UNIFORM, 1, -0.5);
        
        for (size_t i = 0; i < nPointsRand; i++)
        {
            cv::Vec<RealType, 6> a(X(i), Y(i), Z(i), theta(i), phi(i), 
                    lambda(i));
            a = a / cv::norm(a) * sqrt(12.59158724374398);
            X(i) = a(1); Y(i) = a(2); Z(i) = a(3);
            theta(i) = a(4); phi(i) = a(5); lambda(i) = a(6);
        }
        
        rndMat = RealMatrix::zeros(6, nPointsRand);
        X.copyTo(rndMat.row(0));
        Y.copyTo(rndMat.row(1));
        Z.copyTo(rndMat.row(2));
        theta.copyTo(rndMat.row(3));
        phi.copyTo(rndMat.row(4));
        lambda.copyTo(rndMat.row(5));
    }
}
