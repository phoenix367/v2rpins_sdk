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
            X(i) = a(0); Y(i) = a(1); Z(i) = a(2);
            theta(i) = a(3); phi(i) = a(4); lambda(i) = a(5);
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

    RealMatrix22 jacob_undistor_fm(const CameraParams& cam, 
            const RealMatrix21& uvd)
    {
        RealType ud = uvd(0);
        RealType vd = uvd(1);
        RealType xd = (uvd(0) - cam.Cx) * cam.dx;
        RealType yd = (uvd(1) - cam.Cy) * cam.dy;

        RealType rd2 = xd * xd + yd * yd;
        RealType rd4 = rd2 * rd2;

        RealType uu_ud = (1 + cam.k1 * rd2 + cam.k2 * rd4) +
                (ud - cam.Cx) * (cam.k1 + 2 * cam.k2 * rd2) *
                (2 * (ud - cam.Cx) * cam.dx * cam.dx);
        RealType vu_vd = (1 + cam.k1 * rd2 + cam.k2 * rd4) + 
                (vd - cam.Cy) * (cam.k1 + 2 * cam.k2 * rd2) *
                (2 * (vd - cam.Cy) * cam.dy * cam.dy);

        RealType uu_vd = (ud - cam.Cx) * (cam.k1 + 2 * cam.k2 * rd2) *
                (2 * (vd - cam.Cy) * cam.dy * cam.dy);
        RealType vu_ud = (vd - cam.Cy) * (cam.k1 + 2 * cam.k2 * rd2) *
                (2 * (ud - cam.Cx) * cam.dx * cam.dx);

        RealMatrix22 J_undistor;
        J_undistor(0, 0) = uu_ud;
        J_undistor(0, 1) = uu_vd;
        J_undistor(1, 0) = vu_ud;
        J_undistor(1, 1) = vu_vd;
        
        return J_undistor;
    }

    RealMatrix21 undistor_fm(const CameraParams& cam, 
            const RealMatrix21& uvd)
    {
        RealType xd = ( uvd(0) - cam.Cx ) * cam.dx;
        RealType yd = ( uvd(1) - cam.Cy ) * cam.dy;

        RealType rd = sqrt( xd * xd + yd * yd );
        RealType rd2 = pow(rd, 2);
        RealType rd4 = pow(rd, 4);
        
        RealType D = 1 + cam.k1 * rd2 + cam.k2 * rd4;
        RealType xu = xd * D;
        RealType yu = yd * D;

        RealMatrix21 uvu;
        uvu(0) = xu / cam.dx + cam.Cx;
        uvu(1) = yu / cam.dy + cam.Cy;   
        
        return uvu;
    }

    RealMatrix61 hinv(const CameraParams& cam, 
            const RealMatrix21& uvd,
            const RealVector& Xv,
            RealType initial_rho)
    {
        RealType fku =  cam.K(0, 0);
        RealType fkv =  cam.K(1, 1);
        RealType U0  =  cam.K(0, 2);
        RealType V0  =  cam.K(1, 2);

        RealMatrix21 uv = undistor_fm(cam, uvd);
        RealType u = uv(0);
        RealType v = uv(1);

        RealVector r_W = Xv(cv::Range(0, 3));
        RealVector q_WR = Xv(cv::Range(3, 7));

        RealType h_LR_x = -(U0 - u) / fku;
        RealType h_LR_y = -(V0 - v) / fkv;
        RealType h_LR_z = 1;

        RealMatrix31 h_LR(h_LR_x, h_LR_y, h_LR_z);

        RealMatrix n = q2r(q_WR) * RealMatrix(h_LR);
        RealType nx = n(0);
        RealType ny = n(1);
        RealType nz = n(2);

        RealMatrix61 newFeature(r_W[0], r_W[1], r_W[2], atan2(nx, nz), 
                atan2(-ny, sqrt(nx * nx + nz * nz)),
                initial_rho);
        
        return newFeature;
    }

    RealMatrix23 dhu_dhrl(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi)
    {
        RealType f = cam.f;
        RealType ku = 1 / cam.dx;
        RealType kv = 1 / cam.dy;
        RealVector rw = Xv_km1_k(cv::Range(0, 3));
        RealMatrix Rrw = q2r(Xv_km1_k(cv::Range(3, 7))).inv();

        RealType theta = yi[3];
        RealType phi = yi[4];
        RealType rho = yi[5];
        RealMatrix31 mi(cos(phi) * sin(theta), -sin(phi), 
                cos(phi) * cos(theta));

        RealMatrix hc = Rrw * ((yi(cv::Range(0, 3)) - rw) * rho + 
                RealMatrix(mi));
        RealType hcx = hc(0);
        RealType hcy = hc(1);
        RealType hcz = hc(2);
        RealMatrix23 a(+f * ku / hcz, 0, -hcx * f * ku / (hcz * hcz),
                       0, +f * kv / hcz, -hcy * f * kv /
                       (hcz * hcz));
        return a;
    }

    RealMatrix22 dhd_dhu(const CameraParams& cam, const RealMatrix21& zi_d)
    {
        RealMatrix22 a = jacob_undistor_fm(cam, zi_d).inv();
        return a;
    }

    RealMatrix23 dh_dhrl(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi)
    {
        RealMatrix23 a = dhd_dhu(cam, zi) * dhu_dhrl(cam, Xv_km1_k, yi);
        return a;
    }
    
    RealMatrix44 dqbar_by_dq()
    {
        RealMatrix44 m;
        m(0, 0) =  1;
        m(1, 1) = -1;
        m(2, 2) = -1;
        m(3, 3) = -1;
        
        return m;
    }

    RealMatrix33 dR_by_dqz(const RealVector& q)
    {
        RealType q0 = q[0];
        RealType qx = q[1];
        RealType qy = q[2];
        RealType qz = q[3];

        RealMatrix33 dR_by_dqzRES(-2 * qz, -2 * q0, 2 * qx,
                                   2 * q0, -2 * qz, 2 * qy,
                                   2 * qx,  2 * qy, 2 * qz);
        return dR_by_dqzRES;
    }

    RealMatrix33 dR_by_dqy(const RealVector& q)
    {
        RealType q0 = q[0];
        RealType qx = q[1];
        RealType qy = q[2];
        RealType qz = q[3];

        RealMatrix33 dR_by_dqyRES(-2 * qy, 2 * qx,  2 * q0,
                                   2 * qx, 2 * qy,  2 * qz,
                                  -2 * q0, 2 * qz, -2 * qy);
        return dR_by_dqyRES;
    }

    RealMatrix33 dR_by_dqx(const RealVector& q)
    {
        RealType q0 = q[0];
        RealType qx = q[1];
        RealType qy = q[2];
        RealType qz = q[3];

        RealMatrix33 dR_by_dqxRES(2 * qx,  2 * qy,  2 * qz,
	                          2 * qy, -2 * qx, -2 * q0,
	                          2 * qz,  2 * q0, -2 * qx);
        return dR_by_dqxRES;
    }

    RealMatrix33 dR_by_dq0(const RealVector& q)
    {
        RealType q0 = q[0];
        RealType qx = q[1];
        RealType qy = q[2];
        RealType qz = q[3];

        RealMatrix33 dR_by_dq0RES( 2 * q0, -2 * qz,  2 * qy,
	                           2 * qz,  2 * q0, -2 * qx,
	                          -2 * qy,  2 * qx,  2 * q0);
        return dR_by_dq0RES;
    }

    RealMatrix34 dRq_times_a_by_dq(const RealVector& q, 
            const RealMatrix31& aMat)
    {
        RealMatrix34 dRq_times_a_by_dqRES;
        
        RealMatrix33 TempR = dR_by_dq0(q);
        RealMatrix31 Temp31 = TempR * aMat;

        for (int i = 0; i < 3; i++)
        {
            dRq_times_a_by_dqRES(i, 0) = Temp31(i);
        }

        TempR = dR_by_dqx(q);
        Temp31 = TempR * aMat;
        for (int i = 0; i < 3; i++)
        {
            dRq_times_a_by_dqRES(i, 1) = Temp31(i);
        }

        TempR = dR_by_dqy(q);
        Temp31 = TempR * aMat;
        for (int i = 0; i < 3; i++)
        {
            dRq_times_a_by_dqRES(i, 2) = Temp31(i);
        }

        TempR = dR_by_dqz(q);
        Temp31 = TempR * aMat;
        for (int i = 0; i < 3; i++)
        {
            dRq_times_a_by_dqRES(i, 3) = Temp31(i);
        }

        return dRq_times_a_by_dqRES;
    }

    RealMatrix34 dhrl_dqwr(const RealVector& Xv_km1_k,
            const RealVector& yi)
    {
        RealVector rw = Xv_km1_k(cv::Range(0, 3));
        RealVector qwr = Xv_km1_k(cv::Range(3, 7));
        RealType lambda = yi[5];
        RealType phi = yi[4];
        RealType theta = yi[3];
        RealMatrix31 mi(cos(phi) * sin(theta), -sin(phi), 
                cos(phi) * cos(theta));

        RealMatrix31 tmp = RealMatrix((yi(cv::Range(0, 3)) - rw) * lambda + 
                RealMatrix(mi));
        auto a = dRq_times_a_by_dq(qconj(qwr), tmp) * dqbar_by_dq();
        return a;
    }

    RealMatrix24 dh_dqwr(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi)
    {
        auto Hi12 = dh_dhrl( cam, Xv_km1_k, yi, zi ) * 
            dhrl_dqwr( Xv_km1_k, yi );
        return Hi12;
    }

    RealMatrix33 dhrl_drw(const RealVector& Xv_km1_k,
            const RealVector& yi)
    {
        RealMatrix a = -q2r( Xv_km1_k(cv::Range(3, 7))).inv() * yi[5];
        
        return a;
    }
    
    RealMatrix23 dh_drw(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi)
    {
        auto Hi11 = dh_dhrl( cam, Xv_km1_k, yi, zi ) * 
            dhrl_drw( Xv_km1_k, yi );
        return Hi11;
    }

    RealMatrix dh_dxv(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi)
    {
        RealMatrix H1 = RealMatrix::zeros(2, 13);
        
        auto H11 = dh_drw(cam, Xv_km1_k, yi, zi);
        auto H12 = dh_dqwr(cam, Xv_km1_k, yi, zi);
        
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < H11.cols; j++)
            {
                H1(i, j) = H11(i, j);
            }

            for (int j = 0; j < H12.cols; j++)
            {
                H1(i, j + H11.cols) = H12(i, j);
            }
        }
        
        return H1;
    }

    RealMatrix36 dhrl_dy(const RealVector& Xv_km1_k,
            const RealVector& yi)
    {
        RealVector rw = Xv_km1_k(cv::Range(0, 3));
        RealMatrix33 Rrw = RealMatrix(q2r( Xv_km1_k(cv::Range(3, 7))).inv());
        RealType lambda = yi[5];
        RealType phi = yi[4];
        RealType theta = yi[3];

        RealMatrix31 dmi_dthetai = Rrw * RealMatrix31(cos(phi) * cos(theta), 0, 
                -cos(phi) * sin(theta));
        RealMatrix31 dmi_dphii = Rrw * RealMatrix31(-sin(phi) * sin(theta), 
                -cos(phi), -sin(phi) * cos(theta));

        RealMatrix a = RealMatrix::zeros(3, 6);
        RealMatrix(lambda * Rrw).copyTo(a(cv::Range(0, 3), cv::Range(0, 3)));
        RealMatrix(dmi_dthetai).copyTo(a(cv::Range(0, 3), cv::Range(3, 4)));
        RealMatrix(dmi_dphii).copyTo(a(cv::Range(0, 3), cv::Range(4, 5)));
        
        RealMatrix31 tmp = RealMatrix(yi(cv::Range(0, 3)) - rw);
        RealMatrix(Rrw * tmp).copyTo(a(cv::Range(0, 3), 
                cv::Range(5, 6)));

        return a;
    }

    RealMatrix26 dh_dy(const CameraParams& cam, const RealVector& Xv_km1_k,
            const RealVector& yi, const RealMatrix21& zi)
    {
        RealMatrix26 a = dh_dhrl(cam, Xv_km1_k, yi, zi) * dhrl_dy(Xv_km1_k, yi);
        return a;
    }
}
