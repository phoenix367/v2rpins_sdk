/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on January 28, 2014, 10:49 PM
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/swab.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <signal.h>
#include <time.h>
#include <termios.h>
#include <list>
#include <iomanip>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/banded.hpp> 
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/bindings/lapack/geev.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/program_options.hpp>

#define HMC5883L_I2C_ADDR   0x1E
#define I2CDEV              "/dev/i2c"

struct CompassMeasurement
{
    float B_x;
    float B_y;
    float B_z;
};

timer_t gTimerid;
int file;
std::list<CompassMeasurement> gMeasurements;
uint gAdapterIndex = 1;
double gMeasurementFrequency = 10.0;
bool gSIMatrixNormalize = true;

using namespace boost::numeric::ublas;
using namespace boost::numeric::bindings::lapack;
namespace po = boost::program_options;

void writeToDevice(int file, const char * buf, int len) 
{
   if (write(file, buf, len) != len) 
   {
      std::cerr << "Can't write to device" << std::endl;
   }
}

void selectDevice(int file, int addr, const char * name) 
{
   if (ioctl(file, I2C_SLAVE, addr) < 0) 
   {
        std::cerr << name << " not present" << std::endl;
   }
}

void start_timer(void)
{
    double period = 1.0 / gMeasurementFrequency;
    double fractpart, intpart;
    fractpart = modf(period, &intpart);
    
    double nsec = fractpart * 1e9;
    
    struct itimerspec value;

    value.it_value.tv_sec = (int) intpart;
    value.it_value.tv_nsec = nsec;

    value.it_interval.tv_sec = (int) intpart;
    value.it_interval.tv_nsec = nsec;

    timer_create (CLOCK_REALTIME, NULL, &gTimerid);
    timer_settime (gTimerid, 0, &value, NULL);
}

void stop_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;

    timer_settime (gTimerid, 0, &value, NULL);
}

void readSensors()
{
    short x, y, z;
    unsigned char buf[16];

    selectDevice(file, HMC5883L_I2C_ADDR, "HMC5883L");
    writeToDevice(file, "\x03", 1);

    if (read(file, buf, 6) != 6) 
    {
        std::cerr << "Unable to read from HMC5883L" << std::endl;
    }
    else
    {
        x = __swab16(*(short*) &buf[0]);
        z = __swab16(*(short*) &buf[2]);
        y = __swab16(*(short*) &buf[4]);
        
        CompassMeasurement m;
        m.B_x = x;
        m.B_y = y;
        m.B_z = z;
        
        gMeasurements.push_back(m);
    }
}

void timer_callback(int sig) 
{
    readSensors();
}

int mygetch ( void ) 
{
    int ch;
    struct termios oldt, newt;

    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

    return ch;
}

 /**
  * Invert a matrix via gauss-jordan algorithm (PARTIAL PIVOT)
  *
  * @param m The matrix to invert. Must be square.
  * @param singular If the matrix was found to be singular, then this
  *        is set to true, else set to false.
  * @return If singular is false, then the inverted matrix is returned.
  *         Otherwise it contains random values.
  */
 template<class T>
 //#define T double /// for debug
 matrix<T>
 gjinverse(const matrix<T> &m, 
           bool &singular)
 {
     using namespace boost::numeric::ublas;

     const int size = m.size1();

     // Cannot invert if non-square matrix or 0x0 matrix.
     // Report it as singular in these cases, and return 
     // a 0x0 matrix.
     if (size != m.size2() || size == 0)
     {
         singular = true;
         matrix<T> A(0,0);
         return A;
     }

     // Handle 1x1 matrix edge case as general purpose 
     // inverter below requires 2x2 to function properly.
     if (size == 1)
     {
         matrix<T> A(1, 1);
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
     matrix<T> A(size, 2*size);
     matrix_range<matrix<T> > Aleft(A, 
                                    range(0, size), 
                                    range(0, size));
     Aleft = m;
     matrix_range<matrix<T> > Aright(A, 
                                     range(0, size), 
                                     range(size, 2*size));
     Aright = identity_matrix<T>(size);

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
                 matrix_row<matrix<T> > rowk(A, k);
                 matrix_row<matrix<T> > rowl(A, l);
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

void calibration()
{    
    matrix<double> D(gMeasurements.size(), 9);
    int i;
    std::list<CompassMeasurement>::iterator it;
    
    for (i = 0, it = gMeasurements.begin(); it != gMeasurements.end(); 
            it++, i++)
    {
        D(i, 0) = it->B_x * it->B_x;
        D(i, 1) = it->B_y * it->B_y;
        D(i, 2) = it->B_z * it->B_z;
        
        D(i, 3) = 2.0f * it->B_x * it->B_y;
        D(i, 4) = 2.0f * it->B_x * it->B_z;
        D(i, 5) = 2.0f * it->B_y * it->B_z;
        
        D(i, 6) = 2.0f * it->B_x;
        D(i, 7) = 2.0f * it->B_y;
        D(i, 8) = 2.0f * it->B_z;
    }
    
    vector<double> ones(gMeasurements.size(), 1.0), v;
    matrix<double> D_t = trans(D);
    
    bool s;
    v = prod(gjinverse(matrix<double>(prod(D_t, D)), s), 
        prod<vector<double> >(D_t, ones));
    
    if (s)
    {
        std::cerr << "Singular matrix found. " << 
                "Terminate calibration process." << std::endl;
        exit(1);
    }
    
    matrix<double> A(4, 4);
    
    A(0, 0) = v(0); A(0, 1) = v(3); A(0, 2) = v(4); A(0, 3) = v(6);
    A(1, 0) = v(3); A(1, 1) = v(1); A(1, 2) = v(5); A(1, 3) = v(7);
    A(2, 0) = v(4); A(2, 1) = v(5); A(2, 2) = v(2); A(2, 3) = v(8);
    A(3, 0) = v(6); A(3, 1) = v(7); A(3, 2) = v(8); A(3, 3) = -1.0;
    
    matrix<double> A_3 = -subrange(A, 0, 3, 0, 3);
    vector<double> centres = prod(gjinverse(A_3, s), subrange(v, 6, 9));
    
    if (s)
    {
        std::cerr << "Singular matrix found. " << 
                "Terminate calibration process." << std::endl;
        exit(1);
    }

    matrix<double> T = identity_matrix<double>(4);
    T(3, 0) = centres(0);
    T(3, 1) = centres(1);
    T(3, 2) = centres(2);
    
    matrix<double> R = prod(matrix<double>(prod(T, A)), 
        matrix<double>(trans(T)));
    matrix<double, column_major> R_scaled = 
        subrange(R, 0, 3, 0, 3) / -R(3, 3);
    matrix<double, column_major> eigvecs(3, 3);
    vector<std::complex<double> > eigvals(3);
    
    geev(R_scaled, eigvals, (matrix<double, column_major>*) 0, 
        &eigvecs, optimal_workspace());
    
    diagonal_matrix<double> rad(3, 3);
    double norm_W = 0;
    for (size_t i = 0; i < 3; i++)
    {
        if (fabs(eigvals[i].imag()) > 1e-10 || eigvals[i].real() <= 0.0)
        {
            std::cerr << "Eigen value is complex. Bad input data" << std::endl;
            exit(1);
        }
        
        rad(i, i) = sqrt(1.0 / eigvals[i].real());
        norm_W = std::max(norm_W, rad(i, i));
    }
    
    matrix<double> W = prod(prod<matrix<double>>(eigvecs, rad), 
        matrix<double>(trans(eigvecs)));
    
    if (!gSIMatrixNormalize)
    {
        W /= norm_W;
    }
    
    matrix<double> W_inv = gjinverse(W, s);
    
    std::cout << "Calibration values:" << std::endl;
    std::cout << "Hard-Iron offset V" << std::endl;
    std::cout << "X offset = " << centres[0] << std::endl;
    std::cout << "Y offset = " << centres[1] << std::endl;
    std::cout << "Z offset = " << centres[2] << std::endl;
    std::cout << std::endl;
    
    std::cout << "Soft-Iron matrix W^-1" << std::endl;
    std::cout << std::setw(15) << W_inv(0, 0) << std::setw(15) << 
            W_inv(0, 1) << std::setw(15) << W_inv(0, 2) << std::endl;
    std::cout << std::setw(15) << W_inv(1, 0) << std::setw(15) << 
            W_inv(1, 1) << std::setw(15) << W_inv(1, 2) << std::endl;
    std::cout << std::setw(15) << W_inv(2, 0) << std::setw(15) << 
            W_inv(2, 1) << std::setw(15) << W_inv(2, 2) << std::endl;
}

/*
 * 
 */
int main(int argc, char** argv) 
{
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("rate,r", po::value<double>(&gMeasurementFrequency)->default_value(10), 
            "set reading frequency in Hz")
        ("adapter,a", po::value<uint>(&gAdapterIndex)->default_value(1), 
            "set I2C adapter index")
        ("si-norm", po::value<bool>(&gSIMatrixNormalize)->default_value(false), 
            "normalize Soft-Iron matrix to receive results in 0..1 range")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) 
    {
        std::cout << desc << std::endl;
        return 1;
    }
    
    if (vm.count("help"))
    {
        if (gMeasurementFrequency < 1.0 || gMeasurementFrequency > 400)
        {
            std::cerr << "Invalid reading rate." << std::endl;
            exit(1);
        }
    }

    std::ostringstream stringStream;
    
    stringStream << I2CDEV << "-" << gAdapterIndex;
    if ((file = open(stringStream.str().c_str(), O_RDWR)) < 0) 
    {
        std::cerr << "Failed to open I2C bus." << std::endl;
        exit(1);
    }
    
    /* initialize HMC5883L */
    selectDevice(file, HMC5883L_I2C_ADDR, "HMC5883L");
    writeToDevice(file, "\x02\x00", 2);
    
    std::cout << "Start HMC5883L magnetometer reading..." << std::endl;
    
    (void) signal(SIGALRM, timer_callback);
    start_timer();
    
    std::cout << "Press any key to stop compass reading" << std::endl;
    mygetch();

    stop_timer();
    close(file);
    
    std::cout << "Readed " << gMeasurements.size() << " samples." << std::endl;
    calibration();
    
    return 0;
}
