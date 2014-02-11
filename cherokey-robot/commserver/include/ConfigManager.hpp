/* 
 * File:   ConfigManager.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 11, 2013, 1:28 AM
 */

#ifndef CONFIGMANAGER_HPP
#define	CONFIGMANAGER_HPP

#include <memory>
#include <boost/program_options/options_description.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/numeric/ublas/matrix.hpp>

struct ConnectionInfo
{
    boost::asio::ip::address ipAddress;
    uint16_t port;
};

struct CompassOffsets
{
    float V_x;
    float V_y;
    float V_z;
};

enum class AHRSAlgorithm
{
    Madgwick,
    Mahony
};

class ConfigManager 
{
private:
    ConfigManager();
    
public:
    virtual ~ConfigManager();
    
    static ConfigManager* getInstance();
    
    void loadConfiguration(const std::string& fileName);
    std::shared_ptr<ConnectionInfo> getConnectionInfo();
    std::shared_ptr<ConnectionInfo> getSensorsConnectionInfo();
    
    std::string getGPSDevice();
    uint32_t getGPSDeviceBaudrate();
    CompassOffsets getCompassOffsets();
    float getGyroThreshold();
    AHRSAlgorithm getAHRSAlgorithm();
    bool isUseMagnetometer();
    boost::numeric::ublas::matrix<float> getSoftIronMatrix();
    
private:
    boost::numeric::ublas::matrix<float> parseMatrixFromString(
        const std::string& str);
    
private:
    static std::unique_ptr<ConfigManager> instance;
    
    static const std::string IP_ADDRESS_KEY;
    static const std::string PORT_KEY;
    static const std::string SENSORS_ADDRESS_KEY;
    static const std::string SENSORS_PORT_KEY;
    static const std::string GPS_SERIAL_DEVICE;
    static const std::string GPS_SERIAL_BAUDRATE;
    static const std::string IMU_COMPASS_X_OFFSET;
    static const std::string IMU_COMPASS_Y_OFFSET;
    static const std::string IMU_COMPASS_Z_OFFSET;
    static const std::string IMU_COMPASS_SOFT_IRON;
    static const std::string IMU_GYRO_THRESHOLD;
    static const std::string IMU_AHRS_ALGO;
    static const std::string IMU_USE_MAGNETOMETER;
    
    boost::program_options::options_description desc;
    std::shared_ptr<ConnectionInfo> connectionInfo;
    std::shared_ptr<ConnectionInfo> sensorsConnection;
    std::string gpsSerial;
    uint32_t gpsSerialBaudrate;
    CompassOffsets compassOffsets;
    float gyroThreshold;
    AHRSAlgorithm ahrsAlgorithm;
    bool useMagnetometer;
    boost::numeric::ublas::matrix<float> softIronMatrix;
};

#endif	/* CONFIGMANAGER_HPP */
