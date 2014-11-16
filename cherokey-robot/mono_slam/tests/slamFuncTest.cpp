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

void slamFuncTest::testjacob_undistor_fmFunction()
{
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

    mslam::RealType uvdData[2] = {
        53, 127
    };
    mslam::RealMatrix21 uvd(uvdData);
    
    mslam::RealType J_undistorData[2 * 2] = {
        1.41862436820537008,
        0.00519215136113831445,
        0.00519215136113831445,
        1.12037719118838375
    };
    mslam::RealMatrix22 J_undistor(J_undistorData);

    auto res = mslam::jacob_undistor_fm(cam, uvd);
    
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - J_undistor)) > 1e-15));
}

void slamFuncTest::testundistor_fmFunction()
{
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

    mslam::RealType uvdData[2] = {
        53, 127
    };
    mslam::RealMatrix21 uvd(uvdData);
    
    mslam::RealType uvuData[2] = {
        40.1024595594033713,
        126.775536185187377
    };
    mslam::RealMatrix21 uvu(uvuData);

    auto res = mslam::undistor_fm(cam, uvd);
    
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - uvu)) > 1e-14));
}

void slamFuncTest::testhinvFunction()
{
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

    mslam::RealType uvdData[2] = {
        233, 157
    };
    mslam::RealMatrix21 uvd(uvdData);
    
    mslam::RealType KData[3 * 3] = {
        194.062500000000028,                   0, 160.223214285714278,
                          0, 194.062500000000028, 128.866071428571445,
                          0,                   0,                   1
    };
    cam.K = mslam::RealMatrix33(KData);

    mslam::RealType newFeatureData[6] = {
        0.00944470513327725324,
        0.00254699924075165894,
        0.00168509619417212117,
        0.3863318373392442,
        -0.134880740673630123,
        1
    };
    mslam::RealMatrix61 newFeature(newFeatureData);

    mslam::RealType XvData[13] = {
        0.00944470513327725324,
        0.00254699924075165894,
        0.00168509619417212117,
        0.999981401328104758,
        0.00372253651322209007,
        0.0048083810381358439,
        0.000468179003860142828,
        0.00944470513327727926,
        0.00254699924075172616,
        0.00168509619417211661,
        0.00744521313538017843,
        0.00961693688994200505,
        0.000936372857725279459
    };
    mslam::RealVector Xv(XvData, 13, true);
    mslam::RealType initial_rho = 1;

    auto res = mslam::hinv(cam, uvd, Xv, initial_rho);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - newFeature)) > 1e-15));
}

void slamFuncTest::testdhu_dhrlFunction()
{
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

    mslam::RealType Xv_km1_kData[13] = {
        0.0188894102665545308,
        0.00509399848150338466,
        0.003370192388344238,
        0.999925605096383951,
        0.0074449815255538624,
        0.0096166408148683543,
        0.000936345103314383301,
        0.00944470513327727926,
        0.00254699924075172616,
        0.00168509619417211661,
        0.00744521313538017843,
        0.00961693688994200505,
        0.000936372857725279459
    };
    mslam::RealVector Xv_km1_k(Xv_km1_kData, 13, true);

    mslam::RealType yiData[6] = {
        0,
        0,
        0,
        -0.555565960359473032,
        0.00884530452744248553,
        1
    };
    mslam::RealVector yi(yiData, 6, true);

    mslam::RealType aData[2 * 3] = {
        232.231688314087194,
        0,
        156.318148434977218,
        0,
        232.231688314087194,
        0.105247335115085827
    };
    mslam::RealMatrix23 a(aData);
    
    auto res = mslam::dhu_dhrl(cam, Xv_km1_k, yi);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - a)) > 1e-15));
}

void slamFuncTest::testdhd_dhuFunction()
{
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

    mslam::RealType zi_dData[2] = {
        45.7922478625165468,
        128.789026284477416
    };
    mslam::RealMatrix21 zi_d(zi_dData);
    
    mslam::RealType aData[2 * 2] = {
        0.666851444095186929,
        -0.000140830948012770978,
        -0.000140830948012770978,
        0.876019904579995456
    };
    mslam::RealMatrix22 a(aData);

    auto res = mslam::dhd_dhu(cam,zi_d);
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - a)) > 1e-15));
}

void slamFuncTest::testdh_dhrlFunction()
{
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

    mslam::RealType ziData[2] = {
        45.7922478625165468,
        128.789026284477416
    };
    mslam::RealMatrix21 zi(ziData);

    mslam::RealType Xv_km1_kData[13] = {
        0.0188894102665545308,
        0.00509399848150338466,
        0.003370192388344238,
        0.999925605096383951,
        0.0074449815255538624,
        0.0096166408148683543,
        0.000936345103314383301,
        0.00944470513327727926,
        0.00254699924075172616,
        0.00168509619417211661,
        0.00744521313538017843,
        0.00961693688994200505,
        0.000936372857725279459
    };
    mslam::RealVector Xv_km1_k(Xv_km1_kData, 13, true);

    mslam::RealType yiData[6] = {
        0,
        0,
        0,
        -0.555565960359473032,
        0.00884530452744248553,
        1
    };
    mslam::RealVector yi(yiData, 6, true);

    mslam::RealType aData[2 * 3] = {
        154.864036716912381,
        -0.0327054088238792479,
        104.240968200068366,
        -0.0327054088238792479,
        203.439581437357901,
        0.0701843274291173991
    };
    mslam::RealMatrix23 a(aData);

    auto res = mslam::dh_dhrl(cam, Xv_km1_k, yi, zi);
    
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - a)) > 1e-13));
}

void slamFuncTest::testdR_by_dqzFunction()
{
    mslam::RealType qData[4] = {
        0.999925605096383951,
        -0.0074449815255538624,
        -0.0096166408148683543,
        -0.000936345103314383301,
    };
    mslam::RealVector q(qData, 4, true);

    mslam::RealType dR_by_dqzRESData[3 * 3] = {
        0.0018726902066287666,
        -1.9998512101927679,
        -0.0148899630511077248,
        1.9998512101927679,
        0.0018726902066287666,
        -0.0192332816297367086,
        -0.0148899630511077248,
        -0.0192332816297367086,
        -0.0018726902066287666
    };
    mslam::RealMatrix33 dR_by_dqzRES(dR_by_dqzRESData);

    auto res = mslam::dR_by_dqz(q);
    
    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - dR_by_dqzRES)) > 1e-15));
}

void slamFuncTest::testdR_by_dqyFunction()
{
    mslam::RealType qData[4] = {
        0.999925605096383951,
        -0.0074449815255538624,
        -0.0096166408148683543,
        -0.000936345103314383301,
    };
    mslam::RealVector q(qData, 4, true);

    mslam::RealType dR_by_dqyRESData[3 * 3] = {
        0.0192332816297367086,
        -0.0148899630511077248,
        1.9998512101927679,
        -0.0148899630511077248,
        -0.0192332816297367086,
        -0.0018726902066287666,
        -1.9998512101927679,
        -0.0018726902066287666,
        0.0192332816297367086
    };
    mslam::RealMatrix33 dR_by_dqyRES(dR_by_dqyRESData);

    auto res = mslam::dR_by_dqy(q);

    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - dR_by_dqyRES)) > 1e-15));
}

void slamFuncTest::testdR_by_dqxFunction()
{
    mslam::RealType qData[4] = {
        0.999925605096383951,
        -0.0074449815255538624,
        -0.0096166408148683543,
        -0.000936345103314383301,
    };
    mslam::RealVector q(qData, 4, true);

    mslam::RealType dR_by_dqxRESData[3 * 3] = {
        -0.0148899630511077248,
        -0.0192332816297367086,
        -0.0018726902066287666,
        -0.0192332816297367086,
        0.0148899630511077248,
        -1.9998512101927679,
        -0.0018726902066287666,
        1.9998512101927679,
        0.0148899630511077248
    };
    mslam::RealMatrix33 dR_by_dqxRES(dR_by_dqxRESData);

    auto res = mslam::dR_by_dqx(q);

    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - dR_by_dqxRES)) > 1e-15));
}

void slamFuncTest::testdR_by_dq0Function()
{
    mslam::RealType qData[4] = {
        0.999925605096383951,
        -0.0074449815255538624,
        -0.0096166408148683543,
        -0.000936345103314383301,
    };
    mslam::RealVector q(qData, 4, true);

    mslam::RealType dR_by_dq0RESData[3 * 3] = {
        1.9998512101927679,
        0.0018726902066287666,
        -0.0192332816297367086,
        -0.0018726902066287666,
        1.9998512101927679,
        0.0148899630511077248,
        0.0192332816297367086,
        -0.0148899630511077248,
        1.9998512101927679
    };
    mslam::RealMatrix33 dR_by_dq0RES(dR_by_dq0RESData);

    auto res = mslam::dR_by_dq0(q);

    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - dR_by_dq0RES)) > 1e-15));
}

void slamFuncTest::testdRq_times_a_by_dqFunction()
{
    mslam::RealType qData[4] = {
        0.999925605096383951,
        -0.0074449815255538624,
        -0.0096166408148683543,
        -0.000936345103314383301,
    };
    mslam::RealVector q(qData, 4, true);

    mslam::RealType aMatData[3] = {
        -0.546293003463796167,
        -0.0139391876674926155,
        0.846198646794745435
    };
    mslam::RealMatrix31 aMat(aMatData);

    mslam::RealType dRq_times_a_by_dqRESData[3 * 4] = {
        -1.10880600476565361,
        0.00681771104003819385,
        1.68197193465731187,
        0.0142533971837316316,
        -0.0142533971837316316,
        -1.68197193465731187,
        0.00681771104003819385,
        -1.10880600476565361,
        1.68197193465731187,
        -0.0142533971837316316,
        1.10880600476565361,
        0.00681771104003819385
    };
    mslam::RealMatrix34 dRq_times_a_by_dqRES(dRq_times_a_by_dqRESData);

    auto res = mslam::dRq_times_a_by_dq(q, aMat);

    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - dRq_times_a_by_dqRES)) > 1e-15));
}

void slamFuncTest::testdhrl_dqwrFunction()
{
    mslam::RealType Xv_km1_kData[13] = {
        0.0188894102665545308,
        0.00509399848150338466,
        0.003370192388344238,
        0.999925605096383951,
        0.0074449815255538624,
        0.0096166408148683543,
        0.000936345103314383301,
        0.00944470513327727926,
        0.00254699924075172616,
        0.00168509619417211661,
        0.00744521313538017843,
        0.00961693688994200505,
        0.000936372857725279459
    };
    mslam::RealVector Xv_km1_k(Xv_km1_kData, 13, true);

    mslam::RealType yiData[6] = {
        0,
        0,
        0,
        -0.555565960359473032,
        0.00884530452744248553,
        1
    };
    mslam::RealVector yi(yiData, 6, true);

    mslam::RealType aData[3 * 4] = {
        -1.10880600476565361,
        -0.00681771104003819385,
        -1.68197193465731187,
        -0.0142533971837316316,
        -0.0142533971837316316,
        1.68197193465731187,
        -0.00681771104003819385,
        1.10880600476565361,
        1.68197193465731187,
        0.0142533971837316316,
        -1.10880600476565361,
        -0.00681771104003819385
    };
    mslam::RealMatrix34 a(aData);

    auto res = mslam::dhrl_dqwr(Xv_km1_k, yi);

    CPPUNIT_ASSERT_EQUAL(0, cv::countNonZero(
            cv::abs(cv::Mat(res - a)) > 1e-15));
}

void slamFuncTest::testdhrl_dqwrFunction()
{
    mslam::RealType Xv_km1_kData[13] = {
        0.0188894102665545308,
        0.00509399848150338466,
        0.003370192388344238,
        0.999925605096383951,
        0.0074449815255538624,
        0.0096166408148683543,
        0.000936345103314383301,
        0.00944470513327727926,
        0.00254699924075172616,
        0.00168509619417211661,
        0.00744521313538017843,
        0.00961693688994200505,
        0.000936372857725279459
    };
    mslam::RealVector Xv_km1_k(Xv_km1_kData, 13, true);

    mslam::RealType yiData[6] = {
        0,
        0,
        0,
        -0.555565960359473032,
        0.00884530452744248553,
        1
    };
    mslam::RealVector yi(yiData, 6, true);
}
