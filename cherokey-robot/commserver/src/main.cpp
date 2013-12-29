/* 
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on November 12, 2013, 11:05 PM
 */

#include <cstdlib>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <string>
#include <google/protobuf/service.h>
#include <zmq.hpp>

#include "ConfigManager.hpp"
#include "Exceptions.hpp"
#include "ConnectionListener.hpp"

const std::string DEFAULT_CONFIG_FILE = "commserver.cfg";

sig_atomic_t child_exit_status; 
zmq::context_t gContext(1);

void clean_up_child_process (int, siginfo_t *info, void *uap) 
{
    /* Clean up the child process.  */ 
    int status; 
    ::wait((void*) &status); 

    /* Store its exit status in a global variable.  */ 
    child_exit_status = status; 
} 

/*
 * 
 */
int main(int argc, char** argv) 
{
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    memset(&action.sa_mask, 0, sizeof(action.sa_mask));
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = &clean_up_child_process;
    
    int rc = sigaction(SIGCHLD, &action, NULL);
    assert(rc == 0);

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
