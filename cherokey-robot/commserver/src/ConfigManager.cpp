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
            "Offset of compass Z axis");
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