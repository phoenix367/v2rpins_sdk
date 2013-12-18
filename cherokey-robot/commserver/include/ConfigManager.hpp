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

class ConfigManager 
{
private:
    ConfigManager();
    
public:
    virtual ~ConfigManager();
    
    static ConfigManager* getInstance();
    
    void loadConfiguration(const std::string& fileName);
    std::shared_ptr<ConnectionInfo> getConnectionInfo();
    
private:
    static std::unique_ptr<ConfigManager> instance;
    
    static const std::string IP_ADDRESS_KEY;
    static const std::string PORT_KEY;
    
    boost::program_options::options_description desc;
    std::shared_ptr<ConnectionInfo> connectionInfo;
};

#endif	/* CONFIGMANAGER_HPP */
