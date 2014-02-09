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
    static const std::string IMU_GYRO_THRESHOLD;
    
    boost::program_options::options_description desc;
    std::shared_ptr<ConnectionInfo> connectionInfo;
    std::shared_ptr<ConnectionInfo> sensorsConnection;
    std::string gpsSerial;
    uint32_t gpsSerialBaudrate;
    CompassOffsets compassOffsets;
    float gyroThreshold;
};

#endif	/* CONFIGMANAGER_HPP */
