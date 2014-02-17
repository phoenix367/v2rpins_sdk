/* 
 * File:   kalman_filter.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on February 17, 2014, 11:37 PM
 */

#include <boost/numeric/ublas/banded.hpp> 
#include <boost/numeric/ublas/lu.hpp>

#include "kalman_filter.hpp"

KalmanFilter::KalmanFilter() 
{
}

KalmanFilter::~KalmanFilter() 
{
}

KalmanFilter::KalmanFilter(
    const boost::numeric::ublas::matrix<double>& first_x,
    const boost::numeric::ublas::matrix<double>& first_p,
    const boost::numeric::ublas::matrix<double>& measurement_noise,
    const boost::numeric::ublas::matrix<double>& process_noise)
: m_measurement_noise(measurement_noise)
, m_p(first_p)
, m_process_noise(process_noise)
, m_x(first_x)
{

}

const boost::numeric::ublas::matrix<double> Inverse(
    const boost::numeric::ublas::matrix<double>& m,
    bool& singular)
{
    using namespace boost::numeric::ublas;

    const int size = m.size1();

    // Cannot invert if non-square matrix or 0x0 matrix.
    // Report it as singular in these cases, and return 
    // a 0x0 matrix.
    if (size != m.size2() || size == 0)
    {
        singular = true;
        matrix<double> A(0,0);
        return A;
    }

    // Handle 1x1 matrix edge case as general purpose 
    // inverter below requires 2x2 to function properly.
    if (size == 1)
    {
        matrix<double> A(1, 1);
        if (m(0,0) == 0.0)
        {
            singular = true;
            return A;
        }
        singular = false;
        A(0,0) = 1/m(0,0);
        return A;
    }

    // Create an augmented matrix A to invert. Assign the
    // matrix to be inverted to the left hand side and an
    // identity matrix to the right hand side.
    matrix<double> A(size, 2*size);
    matrix_range<matrix<double> > Aleft(A, 
                                   range(0, size), 
                                   range(0, size));
    Aleft = m;
    matrix_range<matrix<double> > Aright(A, 
                                    range(0, size), 
                                    range(size, 2*size));
    Aright = identity_matrix<double>(size);

    // Swap rows to eliminate zero diagonal elements.
    for (int k = 0; k < size; k++)
    {
        if ( A(k,k) == 0 ) // XXX: test for "small" instead
        {
            // Find a row(l) to swap with row(k)
            int l = -1;
            for (int i = k+1; i < size; i++) 
            {
                if ( A(i,k) != 0 )
                {
                    l = i; 
                    break;
                }
            }

            // Swap the rows if found
            if ( l < 0 ) 
            {
                std::cerr << "Error:" <<  __FUNCTION__ << ":"
                          << "Input matrix is singular, because cannot find"
                          << " a row to swap while eliminating zero-diagonal.";
                singular = true;
                return Aleft;
            }
            else 
            {
                matrix_row<matrix<double> > rowk(A, k);
                matrix_row<matrix<double> > rowl(A, l);
                rowk.swap(rowl);
            }
        }
    }
     // Doing partial pivot
    for (int k = 0; k < size; k++)
    {
        // normalize the current row
        for (int j = k+1; j < 2*size; j++)
            A(k,j) /= A(k,k);
        A(k,k) = 1;

        // normalize other rows
        for (int i = 0; i < size; i++)
        {
            if ( i != k )  // other rows  // FIX: PROBLEM HERE
            {
                if ( A(i,k) != 0 )
                {
                    for (int j = k+1; j < 2*size; j++)
                        A(i,j) -= A(k,j) * A(i,k);
                    A(i,k) = 0;
                }
            }
        }

    }

    singular = false;
    return Aright;
}

void KalmanFilter::SupplyMeasurement(
    const boost::numeric::ublas::matrix<double>& x)
{
    using boost::numeric::ublas::identity_matrix;
    using boost::numeric::ublas::matrix;
    using boost::numeric::ublas::prod;
    
    bool s;

    /// 1/7) State prediction
    const matrix<double> x_current = m_x;
    /// 2/7) Covariance prediction
    const matrix<double> p_current = m_p + m_process_noise;
    /// 3/7) Innovation (y with a squiggle above it)
    const matrix<double> z_measured = x; //x has noise in it
    const matrix<double> innovation = z_measured - x_current;
    /// 4/7) Innovation covariance (S)
    const matrix<double> innovation_covariance = p_current + 
        m_measurement_noise;
    /// 5/7) Kalman gain (K)
    const matrix<double> kalman_gain
      = prod(p_current,Inverse(innovation_covariance, s));
    /// 6/7) Update state prediction
    m_x = x_current + prod(kalman_gain,innovation);
    /// 7/7) Update covariance prediction
    const identity_matrix<double> my_identity_matrix(kalman_gain.size1());
    m_p = prod(my_identity_matrix - kalman_gain,p_current);
}
