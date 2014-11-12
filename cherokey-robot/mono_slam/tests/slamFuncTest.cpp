/*
 * File:   slamFuncTest.cpp
 * Author: Ivan Gubochkin
 *
 * Created on 13.09.2014, 23:23:17
 */

#include "slamFuncTest.hpp"
#include "mono_slam/declares.hpp"
#include "mono_slam/slam_functions.hpp"
#include "load_util.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(slamFuncTest);

slamFuncTest::slamFuncTest() {
}

slamFuncTest::~slamFuncTest() {
}

void slamFuncTest::setUp() {
}

void slamFuncTest::tearDown() {
}

void slamFuncTest::testHuFunc() 
{
    mslam::RealType yiData[3] = {
        -0.54473790876651229,
        -0.0045914696402309966,
        0.84238048833577783
    };
    mslam::RealType uv_uData[2] = {
        34.729803785608539,
        127.80831654298801
    };
    mslam::RealMatrix31 yi(yiData);
    mslam::RealMatrix uv_u(2, 1, uv_uData);
    
    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.f = 2.1735000000000002;
    
    auto res = mslam::hu(yi, cam);
    
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(res - uv_u) > 1e-14) == 0);
}

void slamFuncTest::testDistortFmFunc() 
{
    mslam::RealType uv_uData[2] = {
        34.729803785608539,
        127.80831654298801
    };
    mslam::RealType uvdData[2] = {
        49.262988088995215,
        127.93081338675638
    };
    mslam::RealMatrix uv_u(2, 1, uv_uData);
    mslam::RealMatrix uvd(2, 1, uvdData);

    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.k1 = 0.063329999999999997;
    cam.k2 = 0.013899999999999999;
    
    auto res = mslam::distort_fm(uv_u, cam);

    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(res - uvd) > 1e-13) == 0);
}

void slamFuncTest::testMFunc()
{
    mslam::RealType a = -0.55425841752082594, b = 0.0091594900973087975;
    mslam::RealType mData[3] = {
        -0.52629080640410209,
        -0.009159362023354033,
        0.85025531058726422
    };
    cv::Matx<mslam::RealType, 3, 1> m(mData);
    
    auto res = mslam::m(a, b);
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(cv::Mat(res - m)) > 1e-15) == 0);
}

void slamFuncTest::testHighInverseDepthFunc()
{
    mslam::RealType yinitData[] = {
        0,
        0,
        0,
        -0.348435644970339964,
        -0.121573649901980524,
        1
    };
    mslam::RealMatrix61 yinit(yinitData);
    
    mslam::RealType t_wcData[] = {
        0.0108888137549603657,
        0.00251060415912785894,
        0.00311901665777174642,
    };
    mslam::RealMatrix31 t_wc(t_wcData);
    
    mslam::RealType r_wcData[] = {
         0.999970880513577187,    0.000357468264786742608,  0.00762301392765437052,
        -0.000294558034737228442, 0.999965906650850833,    -0.00825219798033385854,
        -0.00762570393247167123,  0.00824971226056550599,   0.999936893452358255
    };
    mslam::RealMatrix33 r_wc(r_wcData);

    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.k1 = 0.063329999999999997;
    cam.k2 = 0.013899999999999999;
    cam.f = 2.1735000000000002;
    cam.nRows = 240;
    cam.nCols = 320;
    
    mslam::RealType targetData[] = {
        89.1484448208473736,
        154.018333421641529
    };
    mslam::RealMatrix m(2, 1, targetData);
    
    auto res = mslam::hi_inverse_depth(yinit, t_wc, r_wc, cam);
    CPPUNIT_ASSERT(cv::countNonZero(cv::abs(cv::Mat(res - m)) > 1e-13) == 0);
}

void slamFuncTest::testPredCamMeasurements()
{
    auto x_k_k = test::loadMatrix("tests/data/test_pred_cam/x_k_k.dat");
    mslam::RealVector x_k_k_v(x_k_k, true);
    static const int FEATURES_COUNT = 12;
    
    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.k1 = 0.063329999999999997;
    cam.k2 = 0.013899999999999999;
    cam.f = 2.1735000000000002;
    cam.nRows = 240;
    cam.nCols = 320;

    std::vector<mslam::FeatureInfo> featuresInfo(FEATURES_COUNT);
    for (size_t i = 0; i < FEATURES_COUNT; i++)
    {
        featuresInfo[i].type = mslam::inversedepth;
    }
    
    mslam::predict_camera_measurements(x_k_k_v, cam, featuresInfo);
    
    CPPUNIT_ASSERT(featuresInfo.size() == FEATURES_COUNT);
    for (size_t i = 0; i < FEATURES_COUNT; i++)
    {
        std::ostringstream stream;
        stream << "tests/data/test_pred_cam/h_" << i << ".dat";
        auto h_t = test::loadMatrix(stream.str());
        
        CPPUNIT_ASSERT(cv::countNonZero(cv::abs(h_t.t() - featuresInfo[i].h) > 
                1e-13) == 0);
    }
}

void slamFuncTest::testFindRANSACFeatures()
{
    cv::Mat1b im_k = cv::imread("tests/data/rawoutput0090.pgm");
    CPPUNIT_ASSERT(!im_k.empty());
    
    mslam::CameraParams cam;
    cam.Cx = 160.22321428571428;
    cam.Cy = 128.86607142857144;
    cam.dx = 0.0112;
    cam.dy = 0.0112;
    cam.k1 = 0.063329999999999997;
    cam.k2 = 0.013899999999999999;
    cam.f = 2.1735000000000002;
    cam.nRows = 240;
    cam.nCols = 320;

    mslam::RealMatrix21 search_region_center;
    search_region_center(0) = 110;
    search_region_center(0) = 184;
    
    int initializing_box_semisize[2] = { 60 / 2, 40 / 2 };
    
    mslam::find_ransac_features(search_region_center, im_k, cam,
            initializing_box_semisize);
}

void slamFuncTest::testfvFunction()
{
    mslam::RealType X_k_kData[13] = {
        0.00944470513327697048,
        0.00254699924075205229,
        0.0016850961941712833,
        0.999981401328104647,
        0.00372253651322187237,
        0.00480838103813518037,
        0.000468179003859795124,
        0.00944470513327704854,
        0.00254699924075206356,
        0.0016850961941712833,
        0.00744521313537957301,
        0.00961693688994071094,
        0.000936372857724466308
    };
    mslam::RealVector X_k_k(X_k_kData, 13, true);
    
    mslam::RealType delta_t = 1;
    
    mslam::RealType X_k_km1Data[13] = {
        0.0188894102665540173,
        0.00509399848150411585,
        0.00337019238834256659,
        0.99992560509638384,
        0.00744498152555334112,
        0.00961664081486704111,
        0.000936345103313629347,
        0.00944470513327704854,
        0.00254699924075206356,
        0.0016850961941712833,
        0.00744521313537957301,
        0.00961693688994071094,
        0.000936372857724466308
    };
    mslam::RealMatrix X_k_km1(13, 1, X_k_km1Data);

    auto res = mslam::fv(X_k_k, delta_t, mslam::constant_velocity);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(mslam::v2m(res) - X_k_km1) > 
            1e-15));
}

void slamFuncTest::testdq3_by_dq1Function()
{
    mslam::RealType q2_inData[4] = {
        0.999981401328104647,
        0.00372253651322187237,
        0.00480838103813518037,
        0.000468179003859795124
    };
    mslam::RealVector q2_in(q2_inData, 4, true);

    mslam::RealType  dq3_by_dq1RESData[4 *4] = {
        0.999981401328104647,
        -0.00372253651322187237,
        -0.00480838103813518037,
        -0.000468179003859795124,
        0.00372253651322187237,
        0.999981401328104647,
        -0.000468179003859795124,
        0.00480838103813518037,
        0.00480838103813518037,
        0.000468179003859795124,
        0.999981401328104647,
        -0.00372253651322187237,
        0.000468179003859795124,
        -0.00480838103813518037,
        0.00372253651322187237,
        0.999981401328104647
    };
    mslam::RealMatrix44 dq3_by_dq1(dq3_by_dq1RESData);
    
    auto res = mslam::dq3_by_dq1(q2_in);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res) - cv::Mat(dq3_by_dq1)) > 1e-15));
}

void slamFuncTest::testdqomegadt_by_domegaFunction()
{
    mslam::RealType omegaData[3] = {
        0.00744521313537957301,
        0.00961693688994071094,
        0.000936372857724466308
    };
    mslam::RealVector omega(omegaData, 3, true);
    
    mslam::RealType dqomegadt_by_domegaRESData[4 * 3] = {
        -0.00186129174429258823,
        -0.00240421931691508313,
        -0.000234091763119599703,
        0.499994590517165993,
        -2.98332827145172399e-06,
        -2.90477898632301932e-07,
        -2.98332827145172399e-06,
        0.499993046594720991,
        -3.75208549207909756e-07,
        -2.90477898632301932e-07,
        -3.75208549207909756e-07,
        0.499996863608898001
    };
    mslam::RealMatrix43 dqomegadt_by_domegaRES(dqomegadt_by_domegaRESData);
    
    auto res = mslam::dqomegadt_by_domega(omega, 1);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - dqomegadt_by_domegaRES)) > 1e-15));
}

void slamFuncTest::testfunc_QFunction()
{
    mslam::RealType X_kData[13] = {
        0.00944470513327697048,
        0.00254699924075205229,
        0.0016850961941712833,
        0.999981401328104647,
        0.00372253651322187237,
        0.00480838103813518037,
        0.000468179003859795124,
        0.00944470513327704854,
        0.00254699924075206356,
        0.0016850961941712833,
        0.00744521313537957301,
        0.00961693688994071094,
        0.000936372857724466308
    };
    mslam::RealVector X_k(X_kData, 13, true);
    
    mslam::RealType QData[13 * 13] = {
        4.90000000000000052e-05, 0, 0, 0, 0, 0, 0, 4.90000000000000052e-05, 0, 0, 0, 0, 0,
        0, 4.90000000000000052e-05, 0, 0, 0, 0, 0, 0, 4.90000000000000052e-05, 0, 0, 0, 0,
        0, 0, 4.90000000000000052e-05, 0, 0, 0, 0, 0, 0, 4.90000000000000052e-05, 0, 0, 0,
        0, 0, 0, 1.82260733972358366e-09, -9.11942387968958286e-08, -1.1779508597589258e-07, -1.14693741889388467e-08, 0, 0, 0, -1.82402047503471043e-07, -2.35607699838196562e-07, -2.29404553126432441e-08,
        0, 0, 0, -9.11942387968958286e-08, 1.2249225703219524e-05, -8.03956034759955702e-10, -7.82789324392115447e-11, 0, 0, 0, 2.44989397652121973e-05, -1.20550347631708558e-08, 1.17747672580716211e-07,
        0, 0, 0, -1.17795085975892567e-07, -8.03956034759955702e-10, 1.22488096418974645e-05, -1.01112460239518908e-10, 0, 0, 0, 1.08855939182267464e-08, 2.44986371622903988e-05, -9.12751185408573967e-08,
        0, 0, 0, -1.14693741889388484e-08, -7.82789324391983098e-11, -1.01112460239518908e-10, 1.22498382623863564e-05, 0, 0, 0, -1.17861537694637388e-07, 9.11280396247559009e-08, 2.44993852819873413e-05,
        4.90000000000000052e-05, 0, 0, 0, 0, 0, 0, 4.90000000000000052e-05, 0, 0, 0, 0, 0,
        0, 4.90000000000000052e-05, 0, 0, 0, 0, 0, 0, 4.90000000000000052e-05, 0, 0, 0, 0,
        0, 0, 4.90000000000000052e-05, 0, 0, 0, 0, 0, 0, 4.90000000000000052e-05, 0, 0, 0,
        0, 0, 0, -1.82402047503471043e-07, 2.44989397652121973e-05, 1.08855939182267464e-08, -1.17861537694637388e-07, 0, 0, 0, 4.90000000000000052e-05, 0, 0,
        0, 0, 0, -2.35607699838196562e-07, -1.20550347631708558e-08, 2.44986371622903988e-05, 9.11280396247559009e-08, 0, 0, 0, 0, 4.90000000000000052e-05, 0,
        0, 0, 0, -2.29404553126432441e-08, 1.17747672580716211e-07, -9.12751185408573967e-08, 2.44993852819873413e-05, 0, 0, 0, 0, 0, 4.90000000000000052e-05
    };
    mslam::RealMatrix Q(13, 13, QData);

    mslam::RealType delta_t = 1;
    mslam::RealType SD_A_component_filter = 0.007;
    mslam::RealType SD_alpha_component_filter = 0.007;
    mslam::RealType linear_acceleration_noise_covariance = 
            SD_A_component_filter * delta_t;
    linear_acceleration_noise_covariance *= 
            linear_acceleration_noise_covariance;
    mslam::RealType angular_acceleration_noise_covariance = 
            SD_alpha_component_filter * delta_t;
    angular_acceleration_noise_covariance *=
            angular_acceleration_noise_covariance;
    mslam::RealMatrix Pn = mslam::RealMatrix::zeros(6, 6);
    
    Pn(0, 0) = linear_acceleration_noise_covariance;
    Pn(1, 1) = linear_acceleration_noise_covariance;
    Pn(2, 2) = linear_acceleration_noise_covariance;
    Pn(3, 3) = angular_acceleration_noise_covariance;
    Pn(4, 4) = angular_acceleration_noise_covariance;
    Pn(5, 5) = angular_acceleration_noise_covariance;
    
    mslam::RealMatrix r = mslam::func_Q(X_k, Pn, delta_t,
            mslam::constant_velocity);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(r - Q)) > 1e-15));
}

void slamFuncTest::testdq3_by_dq2Function()
{
    mslam::RealType q1_inData[4] = {
        0.999981400874166759,
        0.00372258348858517645,
        0.00480843863383016625,
        0.000468183526239199406
    };
    mslam::RealVector q1_in(q1_inData, 4, true);

    mslam::RealType  dq3_by_dq2RESData[4 *4] = {
        0.999981400874166759,
        -0.00372258348858517645,
        -0.00480843863383016625,
        -0.000468183526239199406,
        0.00372258348858517645,
        0.999981400874166759,
        0.000468183526239199406,
        -0.00480843863383016625,
        0.00480843863383016625,
        -0.000468183526239199406,
        0.999981400874166759,
        0.00372258348858517645,
        0.000468183526239199406,
        0.00480843863383016625,
        -0.00372258348858517645,
        0.999981400874166759
    };
    mslam::RealMatrix44 dq3_by_dq2(dq3_by_dq2RESData);
    
    auto res = mslam::dq3_by_dq2(q1_in);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res) - cv::Mat(dq3_by_dq2)) > 1e-15));
}

void slamFuncTest::testdfv_by_dxvFunction()
{
    mslam::RealType XvData[13] = {
        0.00944470513327697048,
        0.00254699924075205229,
        0.0016850961941712833,
        0.999981401328104647,
        0.00372253651322187237,
        0.00480838103813518037,
        0.000468179003859795124,
        0.00944470513327704854,
        0.00254699924075206356,
        0.0016850961941712833,
        0.00744521313537957301,
        0.00961693688994071094,
        0.000936372857724466308
    };
    mslam::RealVector Xv(XvData, 13, true);
    
    mslam::RealType dfv_by_dxvRESData[13 * 13] = {
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0.999981400874166759, -0.00372258348858517645, -0.00480843863383016625, -0.000468183526239199406, 0, 0, 0, -0.00372249076537695947, -0.00480832040486115376, -0.000468172557400882493,
        0, 0, 0, 0.00372258348858517645, 0.999981400874166759, 0.000468183526239199406, -0.00480843863383016625, 0, 0, 0, 0.499978362555350886, -0.000246021117615731737, 0.00240301372613706526,
        0, 0, 0, 0.00480843863383016625, -0.000468183526239199406, 0.999981400874166759, 0.00372258348858517645, 0, 0, 0, 0.000222154977922994795, 0.499972186985518319, -0.00186275752124198755,
        0, 0, 0, 0.000468183526239199406, 0.00480843863383016625, -0.00372258348858517645, 0.999981400874166759, 0, 0, 0, -0.00240533750397219125, 0.0018597559107093038, 0.499987454734435455,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    };
    mslam::RealMatrix dfv_by_dxvRES(13, 13, dfv_by_dxvRESData);
    
    mslam::RealType delta_t = 1;

    auto res = mslam::dfv_by_dxv(Xv, delta_t, mslam::constant_velocity);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - dfv_by_dxvRES)) > 1e-15));
}
