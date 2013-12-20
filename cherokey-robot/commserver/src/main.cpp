/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on November 12, 2013, 11:05 PM
 */

#include <cstdlib>

#include <string>
#include "common.pb.h"

#include "ConfigManager.hpp"
#include "Exceptions.hpp"
#include "ConnectionListener.hpp"

const std::string DEFAULT_CONFIG_FILE = "commserver.cfg";

/*
 * 
 */
int main(int argc, char** argv) 
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::string configFile = DEFAULT_CONFIG_FILE;
    
    if (argc > 1)
    {
        
    }
    
    try
    {
        auto instance = ConfigManager::getInstance();

        if (!instance)
        {
            COMM_EXCEPTION(NullPointerException, "Instance of configuration "
                    "manager is NULL");
        }
        
        instance->loadConfiguration(configFile);
        auto ci = instance->getConnectionInfo();
        
        ConnectionListener listener(ci);
        listener.run();
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(-1);
    }
    
    google::protobuf::ShutdownProtobufLibrary();
    
    return 0;
}
