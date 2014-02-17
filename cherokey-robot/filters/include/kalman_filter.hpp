/* 
 * File:   kalman_filter.hpp
 * Author: Ivan Gubochkin
 *
 * Created on February 17, 2014, 11:37 PM
 */

#ifndef KALMAN_FILTER_HPP
#define	KALMAN_FILTER_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix.hpp>

class KalmanFilter 
{
public:
    KalmanFilter();
    virtual ~KalmanFilter();
    
    ///Initialize the filter with a first measurent
    KalmanFilter(
            const boost::numeric::ublas::matrix<double>& first_x,
            const boost::numeric::ublas::matrix<double>& first_p,
            const boost::numeric::ublas::matrix<double>& measurement_noise,
            const boost::numeric::ublas::matrix<double>& process_noise
        );

    ///Give the filter a real measurement, so it will update itself
    void SupplyMeasurement(const boost::numeric::ublas::matrix<double>& x);

    ///Let the filter predict
    const boost::numeric::ublas::matrix<double>& Predict() const 
    { 
        return m_x; 
    }

    ///Let the filter predict
    const boost::numeric::ublas::matrix<double>& PredictCovariance() const 
    { 
        return m_p; 
    }

private:
    //R: Estimated measurement noise: How to estimate this?
    const boost::numeric::ublas::matrix<double> m_measurement_noise;

    ///The (current prediction of the) covariance
    boost::numeric::ublas::matrix<double> m_p;

    //Q: Process noise: How to estimate this?
    const boost::numeric::ublas::matrix<double> m_process_noise;

    ///The (current prediction of the) measurement
    boost::numeric::ublas::matrix<double> m_x;
};

#endif	/* KALMAN_FILTER_HPP */
