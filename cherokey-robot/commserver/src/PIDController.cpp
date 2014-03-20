/* 
 * File:   PIDController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on March 21, 2014, 12:36 AM
 */

#include <memory>

#include "PIDController.hpp"
#include "ConnectionListener.hpp"

std::unique_ptr<PIDController> PIDController::instance;

PIDController::PIDController() 
: CommandSender(ConnectionListener::INTERNAL_COMMAND_ADDR)
, stopVar(true)
{
}

PIDController::~PIDController() 
{
}

PIDController* PIDController::getInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<PIDController>(new PIDController);
    }
    
    return instance.get();
}

void PIDController::run()
{
    while (!stopVar)
    {
        
    }
}

void PIDController::startController()
{
    if (!stopVar)
    {
        return;
    }
    
    controllerThread = std::unique_ptr<std::thread>(new std::thread(
            std::bind(&PIDController::run, this)));
}

void PIDController::stopController()
{
    stopVar = true;
    
    if (controllerThread)
    {
        controllerThread->join();
    }
}
