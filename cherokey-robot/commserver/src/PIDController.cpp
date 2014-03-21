/* 
 * File:   PIDController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on March 21, 2014, 12:36 AM
 */

#include <memory>

#include "PIDController.hpp"
#include "ConnectionListener.hpp"
#include "Exceptions.hpp"

#include "madgwik_ahrs.h"

#include <thread>

std::unique_ptr<PIDController> PIDController::instance;

PIDController::PIDController() 
: CommandSender(ConnectionListener::INTERNAL_COMMAND_ADDR)
, stopVar(true)
, imuReader(nullptr)
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
    if (!imuReader)
    {
        return;
    }
    
    auto delayTime = std::chrono::milliseconds(100);
    auto t = std::chrono::high_resolution_clock::now();

    while (!stopVar)
    {
        auto angles = imuReader->getCurrentAngles();
        
        QUATERNION qIMU;
        Euler2Quaternion(0, 0, angles.yaw * M_PI / 180, &qIMU);

        std::this_thread::sleep_until(t + delayTime);
        t += delayTime;
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
        imuReader = nullptr;
    }
}

void PIDController::setIMUReader(IMUReader* pReader)
{
    if (!pReader)
    {
        COMM_EXCEPTION(NullPointerException, "IMU reader pointer is null");
    }
    
    imuReader = pReader;
}
