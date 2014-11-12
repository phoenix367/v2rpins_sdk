#include "mono_slam/slam_functions.hpp"
#include "mono_slam/vector_function.hpp"

#include "mono_slam/fast.h"
#include "mono_slam/exceptions.hpp"

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
    
    void find_ransac_features(const RealMatrix21& search_region_center,
            const cv::Mat1b& im_k, const CameraParams& cam,
            int initializing_box_semisize[2])
    {
//    cs = fast_corner_detect_9(double(im_k(search_region_center(2)-initializing_box_semisize(2):search_region_center(2)+initializing_box_semisize(2),...
//        search_region_center(1)-initializing_box_semisize(1):search_region_center(1)+initializing_box_semisize(1))),... % the image,
//        100);
//    c = fast_nonmax(double(im_k(search_region_center(2)-initializing_box_semisize(2):search_region_center(2)+initializing_box_semisize(2),...
//        search_region_center(1)-initializing_box_semisize(1):search_region_center(1)+initializing_box_semisize(1))),... % the image,
//        100, cs);
//    all_uv = c';
//    cd ..
//    
//    if ~isempty(all_uv)
//        all_uv = all_uv + [ (- initializing_box_semisize(1) + search_region_center(1) - 1)*ones(1,size(all_uv,2));...
//            (- initializing_box_semisize(2) + search_region_center(2) - 1)*ones(1,size(all_uv,2))];
//    end
//    
//    nPoints=size(all_uv,2);
//    
//    % Are there corners in the box?
//    are_there_corners = not(isempty(all_uv));
        
        int numPoints;
        //cv::Mat1b r = im_k.adjustROI()
        auto xy = fast9_detect_nonmax(im_k.data, im_k.cols, im_k.rows, 
                im_k.step[0], 100, &numPoints);
        free(xy);
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
            
            find_ransac_features(search_region_center, im_k, cam,
                    initializing_box_semisize);
        }
    }

    RealVector fv(const RealVector& X_k_k, RealType delta, 
            PredictionType type)
    {
        assert(X_k_k.size() == 13);
        
        RealVector rW = X_k_k(cv::Range(0, 3));
        RealVector qWR= X_k_k(cv::Range(3, 7));
        RealVector vW = X_k_k(cv::Range(7, 10));
        RealVector wW = X_k_k(cv::Range(10, 13));
        RealVector res(13);
        
        switch (type)
        {
            case constant_orientation:
                break;
            case constant_position:
                break;
            case constant_position_and_orientation:
                break;
            case constant_position_and_orientation_location_noise:
                break;
            case constant_velocity:
                {
                    auto tmp = rW + vW * delta;
                    std::copy(tmp.begin(), tmp.end(), res.begin());
                    tmp = qprod(qWR, v2q(wW * delta));
                    std::copy(tmp.begin(), tmp.end(), res.begin() + 3);
                    std::copy(vW.begin(), vW.end(), res.begin() + 7);
                    std::copy(wW.begin(), wW.end(), res.begin() + 10);
                }
                break;
        }
        
        return res;
    }

    RealMatrix44 dq3_by_dq1(const RealVector& q2_in)
    {
        if (q2_in.size() != 4)
        {
            SLAM_EXCEPTION(IncorrectParamException, 
                    "Quaternion vector size is invalid");
        }
        
        RealType R = q2_in[0]; 
        RealType X = q2_in[1];
        RealType Y = q2_in[2];
        RealType Z = q2_in[3];
 
        RealType dq3_by_dq1RESData[4 * 4] = {
            R, -X, -Y, -Z,
            X,  R, -Z,  Y,
            Y,  Z,  R, -X,
            Z, -Y,  X,  R
        };
        
        RealMatrix44 dq3_by_dq1RES(dq3_by_dq1RESData);
        
        return dq3_by_dq1RES;
    }

    RealType dq0_by_domegaA(RealType omegaA, RealType omega, RealType delta_t)
    {
        RealType dq0_by_domegaARES = (-delta_t / 2.0) * (omegaA / omega) * 
                sin(omega * delta_t / 2.0);
        return dq0_by_domegaARES;
    }

    RealType dqA_by_domegaA(RealType omegaA, RealType omega, RealType delta_t)
    {
        RealType dqA_by_domegaARES = (delta_t / 2.0) * omegaA * omegaA / (omega * omega) 
                        * cos(omega * delta_t / 2.0) 
                        + (1.0 / omega) * (1.0 - omegaA * omegaA / (omega * omega))
                        * sin(omega * delta_t / 2.0);
        return dqA_by_domegaARES;
    }

    RealType dqA_by_domegaB(RealType omegaA, RealType omegaB, RealType omega, 
            RealType delta_t)
    {
        RealType dqA_by_domegaBRES=(omegaA * omegaB / (omega * omega)) * 
                        ( (delta_t / 2.0) * cos(omega * delta_t / 2.0) 
                        - (1.0 / omega) * sin(omega * delta_t / 2.0) );
        return dqA_by_domegaBRES;
    }

    RealMatrix43 dqomegadt_by_domega(const RealVector& omega,
            RealType delta_t)
    {
        RealMatrix43 dqomegadt_by_domegaRES = RealMatrix43::zeros();

        if (omega.size() != 3)
        {
            SLAM_EXCEPTION(IncorrectParamException, 
                    "Vector size is invalid");
        }
        
        RealType omegamod = norm(omega);

        // Use generic ancillary functions to calculate components of Jacobian
        dqomegadt_by_domegaRES(0, 0) = dq0_by_domegaA(omega[0], omegamod, 
                delta_t);
        dqomegadt_by_domegaRES(0, 1) = dq0_by_domegaA(omega[1], omegamod, 
                delta_t);
        dqomegadt_by_domegaRES(0, 2) = dq0_by_domegaA(omega[2], omegamod, 
                delta_t);
        dqomegadt_by_domegaRES(1, 0) = dqA_by_domegaA(omega[0], omegamod, 
                delta_t);
        dqomegadt_by_domegaRES(1, 1) = dqA_by_domegaB(omega[0], omega[1], 
                omegamod, delta_t);
        dqomegadt_by_domegaRES(1, 2) = dqA_by_domegaB(omega[0], omega[2], 
                omegamod, delta_t);
        dqomegadt_by_domegaRES(2, 0) = dqA_by_domegaB(omega[1], omega[0], 
                omegamod, delta_t);
        dqomegadt_by_domegaRES(2, 1) = dqA_by_domegaA(omega[1], omegamod, 
                delta_t);
        dqomegadt_by_domegaRES(2, 2) = dqA_by_domegaB(omega[1], omega[2], 
                omegamod, delta_t);
        dqomegadt_by_domegaRES(3, 0) = dqA_by_domegaB(omega[2], omega[0], 
                omegamod, delta_t);
        dqomegadt_by_domegaRES(3, 1) = dqA_by_domegaB(omega[2], omega[1], 
                omegamod, delta_t);
        dqomegadt_by_domegaRES(3, 2) = dqA_by_domegaA(omega[2], omegamod, 
                delta_t);
        
        return dqomegadt_by_domegaRES;
    }

    RealMatrix func_Q(const RealVector& Xv, const RealMatrix& Pn,
            RealType delta_t, PredictionType type)
    {
        RealMatrix G;
        if (type == constant_position_and_orientation_location_noise)
        {
            
        }
        else
        {
            RealVector omegaOld = Xv(cv::Range(10, 13));
            RealVector qOld = Xv(cv::Range(3, 7));

            G = RealMatrix::zeros(13,6);
            auto r = dq3_by_dq1(qOld) * dqomegadt_by_domega(omegaOld,
                    delta_t);

            for (int i = 0; i < 3; i++)
            {
                G(7 + i, i) = 1;
                G(10 + i, 3 + i) = 1;
                G(i, i) = delta_t;
            }
            
            assert(r.rows == 4 && r.cols == 3);
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    G(3 + i, 3 + j) = r(i, j);
                }
            }
        }
        
        RealMatrix Q = G * Pn * G.t();
        return Q;
    }

    RealMatrix44 dq3_by_dq2(const RealVector& q1_in)
    {
        if (q1_in.size() != 4)
        {
            SLAM_EXCEPTION(IncorrectParamException, 
                    "Quaternion vector size is invalid");
        }
        
        RealType R = q1_in[0]; 
        RealType X = q1_in[1];
        RealType Y = q1_in[2];
        RealType Z = q1_in[3];
 
        RealType dq3_by_dq2RESData[4 * 4] = {
            R, -X, -Y, -Z,
            X,  R,  Z, -Y,
            Y, -Z,  R,  X,
            Z,  Y, -X,  R
        };
        
        RealMatrix44 dq3_by_dq2RES(dq3_by_dq2RESData);
        
        return dq3_by_dq2RES;
    }

    RealMatrix dfv_by_dxv(const RealVector& Xv, RealType delta_t, 
            PredictionType type)
    {
        RealVector omegaOld = Xv(cv::Range(10, 13));
        RealVector qOld = Xv(cv::Range(3, 7));

        RealMatrix dfv_by_dxvRES = RealMatrix::eye(13, 13);

        RealVector qwt = v2q(omegaOld * delta_t);
        RealMatrix44 dqwt = dq3_by_dq2(qwt);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                dfv_by_dxvRES(3 + i, 3 + j) = dqwt(i, j);
            }
        }
        
        if (type == constant_velocity)
        {
            auto tmp = dq3_by_dq1(qOld) * dqomegadt_by_domega(omegaOld, 
                    delta_t);
            for (int j = 0; j < 3; j++)
            {
                dfv_by_dxvRES(j, 7 + j) = delta_t;
                
                for (int i = 0; i < 4; i++)
                {
                    dfv_by_dxvRES(3 + i, 10 + j) = tmp(i, j);
                }
            }
        }
        
        return dfv_by_dxvRES;
    }
}
