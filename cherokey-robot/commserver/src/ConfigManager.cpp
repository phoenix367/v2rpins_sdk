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

ConfigManager::ConfigManager() 
: desc("Options")
{
    desc.add_options()
        (IP_ADDRESS_KEY.c_str(), boost::program_options::value<std::string>()->required(), 
            "IP Address")
        (PORT_KEY.c_str(), boost::program_options::value<uint16_t>()->required(), 
            "Port")
        (SENSORS_ADDRESS_KEY.c_str(), boost::program_options::value<std::string>(),
            "Sensors publisher IP Address")
        (SENSORS_PORT_KEY.c_str(), boost::program_options::value<std::string>(),
            "Sensors publisher Port");
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
    }
    catch (std::exception& e)
    {
        COMM_EXCEPTION(ConfigurationException, e.what());
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
