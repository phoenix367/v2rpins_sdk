#include "mono_slam/slam_functions.hpp"
#include "mono_slam/vector_function.hpp"

#include "mono_slam/fast.h"

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
    
    RealMatrix hu(const RealMatrix31& yi, const CameraParams& cam)
    {
        RealType u0 = cam.Cx;
        RealType v0 = cam.Cy;
        RealType f  = cam.f;
        RealType ku = 1.0 / cam.dx;
        RealType kv = 1.0 / cam.dy;

        RealMatrix uv_u = RealMatrix::zeros(2, yi.cols);

        for (int i = 0; i < yi.cols; i++)
        {
            uv_u(0, i) = u0 + (yi(0, i) / yi(2, i)) * f * ku;
            uv_u(1, i) = v0 + (yi(1, i) / yi(2, i)) * f * kv;
        }
        
        return uv_u;
    }
    
    RealMatrix distort_fm(const RealMatrix& uv, const CameraParams& cam)
    {
        RealMatrix uvd;
        
        RealType Cx = cam.Cx;
        RealType Cy = cam.Cy;
        RealType k1 = cam.k1;
        RealType k2 = cam.k2;
        RealType dx = cam.dx;
        RealType dy = cam.dy;

        RealMatrix xu = (uv.row(0)- Cx) * dx;
        RealMatrix yu = (uv.row(1)- Cy) * dy;

        RealMatrix ru, rd, xu_2, yu_2;
        cv::pow(xu, 2, xu_2);
        cv::pow(yu, 2, yu_2);
        cv::sqrt(xu_2 + yu_2, ru);

        RealMatrix ru_2, ru_4;
        cv::pow(ru, 2, ru_2);
        cv::pow(ru, 4, ru_4);
        cv::divide(ru, 1 + k1 * ru_2 + k2 * ru_4, rd);

        for (int k = 0; k < 10; k++)
        {
            RealMatrix rd_2, rd_3, rd_4, rd_5;
            cv::pow(rd, 2, rd_2);
            cv::pow(rd, 3, rd_3);
            cv::pow(rd, 4, rd_4);
            cv::pow(rd, 5, rd_5);
            
            RealMatrix f = rd + k1 * rd_3 + k2 * rd_5 - ru;
            RealMatrix f_p = 1 + 3 * k1 * rd_2 + 5 * k2 * rd_4;
            
            RealMatrix tmp;
            cv::divide(f, f_p, tmp);
            rd = rd - tmp;
        }

        RealMatrix rd_2, rd_4;
        cv::pow(rd, 2, rd_2);
        cv::pow(rd, 4, rd_4);
        
        RealMatrix D = 1 + k1 * rd_2 + k2 * rd_4;
        RealMatrix xd, yd;
        cv::divide(xu, D, xd);
        cv::divide(yu, D, yd);

        cv::vconcat(xd / dx + Cx, yd / dy + Cy, uvd);
        return uvd;
    }
    
    RealMatrix31 m(RealType a, RealType b)
    {
        RealType theta = a, phi = b;
        RealType cphi = cos(phi);
        RealMatrix31 r;
        
        r(0) = cphi * sin(theta);
        r(1) = -sin(phi);
        r(2) = cphi * cos(theta);
        
        return r;
    }

    RealMatrix hi_inverse_depth(const RealMatrix61& yinit,
            const RealMatrix31& t_wc,
            const RealMatrix33& r_wc,
            const CameraParams& cam)
    {
        static const RealType c180_PI = 180 / CV_PI;
        
        RealMatrix33 r_cw = r_wc.t();

        RealMatrix31 yi(yinit(0), yinit(1), yinit(2));
        RealType theta = yinit(3);
        RealType phi = yinit(4);
        RealType rho = yinit(5);

        RealMatrix31 mi = m( theta,phi );
        RealMatrix31 hrl = r_cw * ((yi - t_wc) * rho + mi);
        
        if ((atan2( hrl(0), hrl(2) ) * c180_PI < -60) ||
            (atan2( hrl(0), hrl(2) ) * c180_PI > 60) ||
            (atan2( hrl(1), hrl(2) ) * c180_PI < -60) ||
            (atan2( hrl(1), hrl(2) ) * c180_PI > 60))
        {
            return RealMatrix();
        }
        
        // Image coordinates
        auto uv_u = hu( hrl, cam );
        // Add distortion
        auto uv_d = distort_fm(uv_u, cam);
        if ((uv_d(0) >= 0) && (uv_d(0) < cam.nCols ) && (uv_d(1) > 0) && 
            (uv_d(1) < cam.nRows))
        {
            return uv_d;
        }
        else
        {
            return RealMatrix();
        }
    }

    void predict_camera_measurements(const RealVector& x_k_k, 
            const CameraParams& cam,
            std::vector<FeatureInfo>& features_info)
    {
        RealMatrix31 t_wc = v2m(x_k_k(cv::Range(0, 3)));
        RealMatrix33 r_wc = q2r(x_k_k(cv::Range(3, 7)));
        RealVector features = x_k_k(cv::Range(13, x_k_k.size()));
        
        for (size_t i = 0; i < features_info.size(); i++)
        {
            if (features_info[i].type == cartesian)
            {
                
            }
            else if (features_info[i].type == inversedepth)
            {
                RealMatrix61 yi = v2m(features(cv::Range(0, 6)));
                features = features(cv::Range(6, features.size()));
                auto hi = hi_inverse_depth( yi, t_wc, r_wc, cam);
                if (!hi.empty())
                {
                    features_info[i].h = hi;
                }
            }
        }
    }

    void initialize_a_feature(int step, const CameraParams& cam, 
            const cv::Mat1b& im_k, 
            std::vector<FeatureInfo>& features_info,
            EKF& filter, RealMatrix& uv)
    {
        int half_patch_size_when_initialized = 20;
        int excluded_band = half_patch_size_when_initialized + 1;
        int max_initialization_attempts = 1;
        int initializing_box_semisize[2] = { 60 / 2, 40 / 2 };
        RealType initial_rho = 1;
        RealType std_rho = 1;

        RealType std_pxl = filter.getStdZ();
        predict_camera_measurements(filter.getX(), cam, features_info);
        
        RealMatrix uvPred(2, features_info.size());
        for (size_t i = 0; i < features_info.size(); i++)
        {
            features_info[i].h.copyTo(uvPred.col(i));
        }
        
        cv::RNG r;
        bool detected_new = false;
        for (int i = 0; i < max_initialization_attempts; i++)
        {
            if (detected_new)
            {
                break;
            }
            
            RealMatrix21 search_region_center;
            r.fill(search_region_center, cv::RNG::UNIFORM, 0, 1);
            search_region_center(0) = round(search_region_center(0) * (
                    cam.nCols - 2 * excluded_band - 2 * initializing_box_semisize[0]))
                    + excluded_band + initializing_box_semisize[0];
            search_region_center(1) = round(search_region_center(1) * (
                    cam.nRows - 2 * excluded_band - 2 * initializing_box_semisize[1]))
                    + excluded_band + initializing_box_semisize[1];
            
            //c = fast9_detect_nonmax(im_k.data, im_k.cols, )
        }
    }
}
