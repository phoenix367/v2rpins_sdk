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
#include "common.pb.h"

#include <thread>

namespace cc = cherokey::common;

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
    
    QUATERNION qTarget;
    Euler2Quaternion(0, 0, 15 * M_PI / 180, &qTarget);
    bool commandDone = false;
    float rotLeftFactor = INFINITY, rotRightFactor = INFINITY;
    int rotDirection = 0;

    while (!stopVar)
    {
        if (!commandDone)
        {
            auto angles = imuReader->getCurrentAngles();

            QUATERNION qIMU, qIMUConj, qRot;
            Euler2Quaternion(0, 0, angles.yaw * M_PI / 180, &qIMU);
            QuaternionConj(&qIMU, &qIMUConj);
            QuaternionProd(&qTarget, &qIMUConj, &qRot);

            float rollRot, pitchRot, yawRot;
            Quaternion2Euler(&qRot, &rollRot, &pitchRot, &yawRot);

            yawRot *= 180 / M_PI;

            if (fabs(yawRot) < 5)
            {
                commandDone = true;
                stopRotation();
            }
            else
            {
                doRotation(yawRot, rotLeftFactor, rotRightFactor,
                        rotDirection);
            }
        }
        //std::cout << rollRot << " " << pitchRot << " " << yawRot << std::endl;

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
    
    stopVar = false;
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

void PIDController::stopRotation()
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_move_action();
    cc::RunDriveGroup *groupA = moveCmd->mutable_run_group_a();
    
    groupA->set_direction(cc::RunDriveGroup::FORWARD);   
    groupA->set_power(0);

    cc::RunDriveGroup *groupB = moveCmd->mutable_run_group_b();

    groupB->set_direction(cc::RunDriveGroup::FORWARD);    
    groupB->set_power(0);
    
    sendMessage(commandMessage);
}

void PIDController::doRotation(float angle, float& leftFactor, 
        float& rightFactor, int& rotDirection)
{
    float tmpLeft, tmpRight;
    int tmpDirection;
    float absAngle = fabs(angle);
    
    if (absAngle > 20.0f)
    {
        tmpLeft = tmpRight = 1.0f;
    }
    else if (absAngle <= 20.0f && absAngle > 10.0f)
    {
        tmpLeft = tmpRight = 0.8f;
    }
    else
    {
        tmpLeft = tmpRight = 0.7f;
    }

    if (angle > 0)
    {
        tmpDirection = 1;
    }
    else
    {
        tmpDirection = -1;
    }
    
    if (fabs(tmpLeft - leftFactor) < 1e-5 && 
        fabs(tmpRight - rightFactor) < 1e-5 &&
        tmpDirection == rotDirection)
    {
        return;
    }
    
    leftFactor = tmpLeft;
    rightFactor = tmpRight;
    rotDirection = tmpDirection;
    
    doRotation(leftFactor, rightFactor, rotDirection);
}

void PIDController::doRotation(float leftFactor, float rightFactor, 
        int rotDirection)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_move_action();
    cc::RunDriveGroup *groupA = moveCmd->mutable_run_group_a();
    
    if (rotDirection > 0)
    {
        groupA->set_direction(cc::RunDriveGroup::FORWARD);
    }
    else
    {
        groupA->set_direction(cc::RunDriveGroup::BACKWARD);
    }
    
    groupA->set_power(leftFactor);

    cc::RunDriveGroup *groupB = moveCmd->mutable_run_group_b();

    if (rotDirection > 0)
    {
        groupB->set_direction(cc::RunDriveGroup::BACKWARD);
    }
    else
    {
        groupB->set_direction(cc::RunDriveGroup::FORWARD);
    }
    
    groupB->set_power(rightFactor);
    
    sendMessage(commandMessage);
}
