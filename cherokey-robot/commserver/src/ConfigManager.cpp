/* 
 * File:   ConfigManager.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 11, 2013, 1:28 AM
 */

#include "ConfigManager.hpp"
#include "Exceptions.hpp"

#include <fstream>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string.hpp>

#define MAHONY_ALGO_NAME        "Mahony"
#define MADGWICK_ALGO_NAME      "Madgwick"

std::unique_ptr<ConfigManager> ConfigManager::instance;

const std::string ConfigManager::IP_ADDRESS_KEY = "Connection.IPAddress";
const std::string ConfigManager::PORT_KEY = "Connection.Port";
const std::string ConfigManager::SENSORS_ADDRESS_KEY = "Sensors.IPAddress";
const std::string ConfigManager::SENSORS_PORT_KEY = "Sensors.Port";
const std::string ConfigManager::GPS_SERIAL_DEVICE = "GPS.SerialDevice";
const std::string ConfigManager::GPS_SERIAL_BAUDRATE = "GPS.Baudrate";
const std::string ConfigManager::IMU_COMPASS_X_OFFSET = "IMU.V_x";
const std::string ConfigManager::IMU_COMPASS_Y_OFFSET = "IMU.V_y";
const std::string ConfigManager::IMU_COMPASS_Z_OFFSET = "IMU.V_z";
const std::string ConfigManager::IMU_GYRO_THRESHOLD = "IMU.GyroThreshold";
const std::string ConfigManager::IMU_AHRS_ALGO = "IMU.AHRSAlgo";
const std::string ConfigManager::IMU_USE_MAGNETOMETER = "IMU.UseMagnetometer";
const std::string ConfigManager::IMU_COMPASS_SOFT_IRON = "IMU.SoftIronMatrix";
const std::string ConfigManager::PINS_DRIVE_A_DIRECTION_PIN =
    "Pins.DriveADirectionPin";
const std::string ConfigManager::PINS_DRIVE_B_DIRECTION_PIN =
    "Pins.DriveBDirectionPin";
const std::string ConfigManager::PINS_VIDEO_TX_PWR_PIN =
    "Pins.VideoTxPwrPin";
const std::string ConfigManager::PINS_PWM_A = "Pins.PWM_A";
const std::string ConfigManager::PINS_PWM_B = "Pins.PWM_B";
const std::string ConfigManager::ADC_VOLTAGE_CHANNEL = "ADC.VoltageChannel";
const std::string ConfigManager::ADC_CURRENT_CHANNEL = "ADC.CurrentChannel";

ConfigManager::ConfigManager() 
: desc("Options")
, gpsSerialBaudrate(0)
{
    desc.add_options()
        (IP_ADDRESS_KEY.c_str(), boost::program_options::value<std::string>()->required(), 
            "IP Address")
        (PORT_KEY.c_str(), boost::program_options::value<uint16_t>()->required(), 
            "Port")
        (SENSORS_ADDRESS_KEY.c_str(), boost::program_options::value<std::string>(),
            "Sensors publisher IP Address")
        (SENSORS_PORT_KEY.c_str(), boost::program_options::value<uint16_t>(),
            "Sensors publisher Port")
        (GPS_SERIAL_DEVICE.c_str(), boost::program_options::value<std::string>(),
            "Serial device to connect with GPS receiver")
        (GPS_SERIAL_BAUDRATE.c_str(), boost::program_options::value<uint32_t>(),
            "Desired buadrate of serial device")
        (IMU_COMPASS_X_OFFSET.c_str(), boost::program_options::value<float>(),
            "Offset of compass X axis")
        (IMU_COMPASS_Y_OFFSET.c_str(), boost::program_options::value<float>(),
            "Offset of compass Y axis")
        (IMU_COMPASS_Z_OFFSET.c_str(), boost::program_options::value<float>(),
            "Offset of compass Z axis")
        (IMU_GYRO_THRESHOLD.c_str(), boost::program_options::value<float>(),
            "Gyroscope angular threshold")
        (IMU_AHRS_ALGO.c_str(), boost::program_options::value<std::string>(),
            "AHRS algorithm")
        (IMU_USE_MAGNETOMETER.c_str(), boost::program_options::value<bool>(),
            "Specify use or not magnetometer data is AHRS algorithm")
        (IMU_COMPASS_SOFT_IRON.c_str(), boost::program_options::value<std::string>(),
            "Soft iron transform matrix")
        (PINS_DRIVE_A_DIRECTION_PIN.c_str(), boost::program_options::value<uint32_t>()->required(),
            "Drive A group direction pin")
        (PINS_DRIVE_B_DIRECTION_PIN.c_str(), boost::program_options::value<uint32_t>()->required(),
            "Drive B group direction pin")
        (PINS_VIDEO_TX_PWR_PIN.c_str(), boost::program_options::value<uint32_t>()->required(),
            "Video transmitter power pin")
        (PINS_PWM_A.c_str(), boost::program_options::value<uint32_t>()->required(),
            "PWM channel for A drivers group")
        (PINS_PWM_B.c_str(), boost::program_options::value<uint32_t>()->required(),
            "PWM channel for B drivers group")
        (ADC_VOLTAGE_CHANNEL.c_str(), boost::program_options::value<int32_t>()->required(),
            "ADC channel for voltage sensor")
        (ADC_CURRENT_CHANNEL.c_str(), boost::program_options::value<int32_t>()->required(),
            "ADC channel for current sensor")
        ;
}

ConfigManager::~ConfigManager() 
{
}

ConfigManager* ConfigManager::getInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<ConfigManager>(new ConfigManager());
    }
    
    return instance.get();
}

void ConfigManager::loadConfiguration(const std::string& fileName)
{
    std::ifstream configStream(fileName);
    
    if (!configStream)
    {
        COMM_EXCEPTION(FileException, "Can't open configuration file");
    }
    
    boost::program_options::variables_map vm;
    
    try
    {
        boost::program_options::store(
            boost::program_options::parse_config_file(configStream, desc), vm);

        connectionInfo = std::make_shared<ConnectionInfo>();
        if (vm.count(IP_ADDRESS_KEY))
        {
            connectionInfo->ipAddress = boost::asio::ip::address::from_string(
                    vm[IP_ADDRESS_KEY].as<std::string>());
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "Server IP address is not specified.");
        }

        if (vm.count(PORT_KEY))
        {
            connectionInfo->port = vm[PORT_KEY].as<uint16_t>();
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "Server PORT is not specified.");
        }
        
        sensorsConnection = std::make_shared<ConnectionInfo>();
        if (vm.count(SENSORS_ADDRESS_KEY))
        {
            sensorsConnection->ipAddress = boost::asio::ip::address::from_string(
                    vm[SENSORS_ADDRESS_KEY].as<std::string>());
        }
        else
        {
            sensorsConnection->ipAddress = connectionInfo->ipAddress;
        }
        
        if (vm.count(SENSORS_PORT_KEY))
        {
            sensorsConnection->port = vm[SENSORS_PORT_KEY].as<uint16_t>();
        }
        else
        {
            sensorsConnection->port = connectionInfo->port + 1;
        }
        
        if (vm.count(GPS_SERIAL_DEVICE))
        {
            gpsSerial = vm[GPS_SERIAL_DEVICE].as<std::string>();
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "GPS serial device is not specified.");
        }
        
        if (vm.count(GPS_SERIAL_BAUDRATE))
        {
            gpsSerialBaudrate = vm[GPS_SERIAL_BAUDRATE].as<uint32_t>();
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "GPS serial device baudrate is not specified.");
        }
        
        if (vm.count(IMU_COMPASS_X_OFFSET))
        {
            compassOffsets.V_x = vm[IMU_COMPASS_X_OFFSET].as<float>();
        }
        else
        {
            compassOffsets.V_x = 0.0f;
        }

        if (vm.count(IMU_COMPASS_Y_OFFSET))
        {
            compassOffsets.V_y = vm[IMU_COMPASS_Y_OFFSET].as<float>();
        }
        else
        {
            compassOffsets.V_y = 0.0f;
        }

        if (vm.count(IMU_COMPASS_Z_OFFSET))
        {
            compassOffsets.V_z = vm[IMU_COMPASS_Z_OFFSET].as<float>();
        }
        else
        {
            compassOffsets.V_z = 0.0f;
        }
        
        if (vm.count(IMU_GYRO_THRESHOLD))
        {
            gyroThreshold = vm[IMU_GYRO_THRESHOLD].as<float>();
            
            if (gyroThreshold < 0.0f)
            {
                COMM_EXCEPTION(ConfigurationException, 
                        "Invalid gyroscope yaw threshold");
            }
        }
        else
        {
            gyroThreshold = 0.0f;
        }
        
        if (vm.count(IMU_AHRS_ALGO))
        {
            if (boost::iequals(MADGWICK_ALGO_NAME, 
                    vm[IMU_AHRS_ALGO].as<std::string>()))
            {
                ahrsAlgorithm = AHRSAlgorithm::Madgwick;
            }
            else if (boost::iequals(MAHONY_ALGO_NAME, 
                    vm[IMU_AHRS_ALGO].as<std::string>()))
            {
                ahrsAlgorithm = AHRSAlgorithm::Mahony;
            }
            else
            {
                COMM_EXCEPTION(ConfigurationException, 
                        "Invalid AHRS algorithm type");
            }
        }
        else
        {
            ahrsAlgorithm = AHRSAlgorithm::Madgwick;
        }
        
        if (vm.count(IMU_USE_MAGNETOMETER))
        {
            useMagnetometer = vm[IMU_USE_MAGNETOMETER].as<bool>();
        }
        else
        {
            useMagnetometer = false;
        }
        
        if (vm.count(IMU_COMPASS_SOFT_IRON))
        {
            softIronMatrix = parseMatrixFromString(
                    vm[IMU_COMPASS_SOFT_IRON].as<std::string>());
            
            if (softIronMatrix.size1() != 3 && softIronMatrix.size2() != 3)
            {
                COMM_EXCEPTION(ConfigurationException, 
                        "Invalid Soft-Iron matrix size");
            }
        }
        else
        {
            softIronMatrix = boost::numeric::ublas::identity_matrix<float>(3, 
                    3);
        }
        
        if (vm.count(PINS_DRIVE_A_DIRECTION_PIN))
        {
            pins.driveADirectionPin = pc::gpioPinFromIndex(
                    vm[PINS_DRIVE_A_DIRECTION_PIN].as<uint32_t>());
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "Pin for drive A direction isn't specified");
        }

        if (vm.count(PINS_DRIVE_B_DIRECTION_PIN))
        {
            pins.driveBDirectionPin = pc::gpioPinFromIndex(
                    vm[PINS_DRIVE_B_DIRECTION_PIN].as<uint32_t>());
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "Pin for drive B direction isn't specified");
        }

        if (vm.count(PINS_VIDEO_TX_PWR_PIN))
        {
            pins.videoTxPowerPin = pc::gpioPinFromIndex(
                    vm[PINS_VIDEO_TX_PWR_PIN].as<uint32_t>());
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "Pin for video transmitter power isn't specified");
        }

        if (vm.count(PINS_PWM_A))
        {
            pins.channelA = pc::pwmFromIndex(
                    vm[PINS_PWM_A].as<uint32_t>());
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "PWM channel isn't specified");
        }

        if (vm.count(PINS_PWM_B))
        {
            pins.channelB = pc::pwmFromIndex(
                    vm[PINS_PWM_B].as<uint32_t>());
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "PWM channel isn't specified");
        }

        if (vm.count(ADC_VOLTAGE_CHANNEL))
        {
            adcInfo.voltageChannel = vm[ADC_VOLTAGE_CHANNEL].as<int32_t>();
            
            if (adcInfo.voltageChannel < 0 || adcInfo.voltageChannel > 5)
            {
                COMM_EXCEPTION(ConfigurationException, 
                    "Incorrect ADC channel");
            }
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "ADC channel for voltage sensor isn't specified");
        }

        if (vm.count(ADC_CURRENT_CHANNEL))
        {
            adcInfo.currentChannel = vm[ADC_CURRENT_CHANNEL].as<int32_t>();
            
            if (adcInfo.currentChannel < 0 || adcInfo.currentChannel > 5)
            {
                COMM_EXCEPTION(ConfigurationException, 
                    "Incorrect ADC channel");
            }
        }
        else
        {
            COMM_EXCEPTION(ConfigurationException, 
                    "ADC channel for current sensor isn't specified");
        }
    }
    catch (Exception&)
    {
        throw;
    }
    catch (std::exception& e)
    {
        COMM_EXCEPTION(InternalError, e.what());
    }
}

std::shared_ptr<ConnectionInfo> ConfigManager::getConnectionInfo()
{
    return connectionInfo;
}

std::shared_ptr<ConnectionInfo> ConfigManager::getSensorsConnectionInfo()
{
    return sensorsConnection;
}

std::string ConfigManager::getGPSDevice()
{
    return gpsSerial;
}

uint32_t ConfigManager::getGPSDeviceBaudrate()
{
    return gpsSerialBaudrate;
}

CompassOffsets ConfigManager::getCompassOffsets()
{
    return compassOffsets;
}

float ConfigManager::getGyroThreshold()
{
    return gyroThreshold;
}

AHRSAlgorithm ConfigManager::getAHRSAlgorithm()
{
    return ahrsAlgorithm;
}

bool ConfigManager::isUseMagnetometer()
{
    return useMagnetometer;
}

boost::numeric::ublas::matrix<float> ConfigManager::getSoftIronMatrix()
{
    return softIronMatrix;
}

boost::numeric::ublas::matrix<float> ConfigManager::parseMatrixFromString(
    const std::string& str)
{
    boost::numeric::ublas::matrix<float> m;
    
    if (!(boost::starts_with(str, "[") && boost::ends_with(str, "]")))
    {
        COMM_EXCEPTION(ConfigurationException, "Can't parse matrix string");
    }
    
    std::string unbrackedStr = str.substr(1, str.size() - 2);
    std::istringstream rowStream(unbrackedStr);
    std::vector<std::vector<float>> vectorizedMatrix;
    
    while (!rowStream.eof())
    {
        std::string columnStr;
        std::getline(rowStream, columnStr, ';');
        
        std::istringstream columnStream(columnStr);
        std::vector<float> columnVector;
        while (!columnStream.eof())
        {
            float item;
            columnStream >> item;
            
            columnVector.push_back(item);
        }
        
        vectorizedMatrix.push_back(columnVector);
    }
    
    // Check loaded matrix
    
    if (!vectorizedMatrix.empty())
    {
        size_t columnSize = vectorizedMatrix[0].size();
        size_t rowSize = vectorizedMatrix.size();
        
        for (size_t i = 1; i < rowSize; i++)
        {
            if (columnSize != vectorizedMatrix[i].size())
            {
                COMM_EXCEPTION(ConfigurationException, 
                        "Incorrect matrix string");
            }
        }
        
        m = boost::numeric::ublas::matrix<float>(rowSize, columnSize);
        
        for (size_t i = 0; i < rowSize; i++)
        {
            for (size_t j = 0; j < columnSize; j++)
            {
                m(i, j) = vectorizedMatrix[i][j];
            }
        }
    }
    
    return m;
}

PinsInfo ConfigManager::getPinsInfo()
{
    return pins;
}

AdcInfo ConfigManager::getAdcInfo()
{
    return adcInfo;
}

VoltageSensorInfo ConfigManager::getVoltageSensorInfo()
{
    return voltageInfo;
}
