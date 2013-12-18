/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on November 12, 2013, 11:05 PM
 */

#include <cstdlib>

#include <string>

#include "ConfigManager.hpp"
#include "Exceptions.hpp"
#include "ConnectionListener.hpp"

const std::string DEFAULT_CONFIG_FILE = "commserver.cfg";

/*
 * 
 */
int main(int argc, char** argv) 
{
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
    
    /*
    // Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");

    while (1) 
    {
        // Send message to all subscribers
        zmq::message_t message(20);
        //snprintf((char *) message.data(), 20,
        //        "%05d %d %d", zipcode, temperature, relhumidity);
        publisher.send(message);

    }
    */
    
    return 0;
}
