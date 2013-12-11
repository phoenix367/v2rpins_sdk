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

class ConfigManager 
{
private:
    ConfigManager();
    
public:
    virtual ~ConfigManager();
    
    static ConfigManager* getInstance();
    
    void loadConfiguration(const std::string& fileName);
    
private:
    static std::unique_ptr<ConfigManager> instance;
    
    boost::program_options::options_description desc;
};

#endif	/* CONFIGMANAGER_HPP */

