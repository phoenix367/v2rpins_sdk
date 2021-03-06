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

#include "pincontroller/global.hpp"

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

struct PinsInfo
{
    pc::GPIO_PIN driveADirectionPin;
    pc::GPIO_PIN driveBDirectionPin;
    pc::GPIO_PIN videoTxPowerPin;
    pc::GPIO_PIN calibIndPin;
    pc::PWM_CHANNEL channelA;
    pc::PWM_CHANNEL channelB;
};

struct AdcInfo
{
    int voltageChannel;
    int currentChannel;
};

struct VoltageSensorInfo
{
    double voltageScale;
    double currentScale;
    double currentOffset;
    uint32_t measurementRate;
};

struct DeadZone
{
    float upperLimit;
    float lowerLimit;
};

struct RotationPIDConstants
{
    float Kp;
    float Ki;
    float Kd;
    uint32_t rotationTimeout;
    float rotationPrecession;
    
    DeadZone deadZone;
};

struct PIDInfo
{
    uint32_t commandSampleTime;
    uint32_t waitSampleTime;
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
    PinsInfo getPinsInfo();
    AdcInfo getAdcInfo();
    VoltageSensorInfo getVoltageSensorInfo();
    uint32_t getGPSSerialTimeout();
    RotationPIDConstants getRotationPIDInfo();
    PIDInfo getCommonPIDInfo();
    
private:
    boost::numeric::ublas::matrix<float> parseMatrixFromString(
        const std::string& str);
    DeadZone parseDeadZone(const std::string& str);
    
private:
    static std::unique_ptr<ConfigManager> instance;
    
    static const std::string IP_ADDRESS_KEY;
    static const std::string PORT_KEY;
    static const std::string SENSORS_ADDRESS_KEY;
    static const std::string SENSORS_PORT_KEY;
    static const std::string GPS_SERIAL_DEVICE;
    static const std::string GPS_SERIAL_BAUDRATE;
    static const std::string GPS_SERIAL_TIMEOUT;
    static const std::string IMU_COMPASS_X_OFFSET;
    static const std::string IMU_COMPASS_Y_OFFSET;
    static const std::string IMU_COMPASS_Z_OFFSET;
    static const std::string IMU_COMPASS_SOFT_IRON;
    static const std::string IMU_GYRO_THRESHOLD;
    static const std::string IMU_AHRS_ALGO;
    static const std::string IMU_USE_MAGNETOMETER;
    static const std::string PINS_DRIVE_A_DIRECTION_PIN;
    static const std::string PINS_DRIVE_B_DIRECTION_PIN;
    static const std::string PINS_VIDEO_TX_PWR_PIN;
    static const std::string PINS_CALIB_IND_PIN;
    static const std::string PINS_PWM_A;
    static const std::string PINS_PWM_B;
    static const std::string ADC_VOLTAGE_CHANNEL;
    static const std::string ADC_CURRENT_CHANNEL;
    static const std::string VOLTAGE_VOLTAGE_SCALE;
    static const std::string VOLTAGE_CURRENT_SCALE;
    static const std::string VOLTAGE_CURRENT_OFFSET;
    static const std::string VOLTAGE_MEASUREMENT_RATE;
    static const std::string ROTATION_PID_KP;
    static const std::string ROTATION_PID_KI;
    static const std::string ROTATION_PID_KD;
    static const std::string ROTATION_PID_TIMEOUT;
    static const std::string ROTATION_PID_PRECESSION;
    static const std::string ROTATION_PID_DEAD_ZONE;
    static const std::string COMMON_PID_COMMAND_ST;
    static const std::string COMMON_PID_WAIT_ST;
    
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
    PinsInfo pins;
    AdcInfo adcInfo;
    VoltageSensorInfo voltageInfo;
    uint32_t gpsSerialTimeout;
    RotationPIDConstants rotationPID;
    PIDInfo commonPIDInfo;
};

#endif	/* CONFIGMANAGER_HPP */
