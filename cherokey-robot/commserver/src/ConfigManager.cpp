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

std::unique_ptr<ConfigManager> ConfigManager::instance;

ConfigManager::ConfigManager() 
: desc("Connection")
{
    desc.add_options()
        ("IPAddress,i","IP Address")
        ("Port,p","Port");
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
    
    boost::program_options::parse_config_file(configStream,
            desc);
}
