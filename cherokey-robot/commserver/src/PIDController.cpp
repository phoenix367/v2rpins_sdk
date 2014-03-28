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

#include "common.pb.h"
#include "notifications.pb.h"

#include <thread>

namespace cc = cherokey::common;
namespace cn = cherokey::notifications;

std::unique_ptr<PIDController> PIDController::instance;

PIDController::PIDController() 
: stopVar(true)
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
    
    auto commandDelay = std::chrono::milliseconds(100);
    auto pidDelay = std::chrono::milliseconds(50);
    auto t = std::chrono::high_resolution_clock::now();
    
    while (!stopVar)
    {
        auto cPtr = getCommand();
        
        if (cPtr)
        {
            std::unique_ptr<CommandImpl> command;
            
            switch (cPtr->getCommandType())
            {
                case CommandType::rotating:
                    {
                        auto cmd = cPtr->commandInstance<RotateCommand>();
                        float angle = cmd->getRotationAngle();

                        std::cout << 
                                "Start executing rotation command, angle:" <<
                                angle << std::endl;
                        
                        command = std::unique_ptr<CommandImpl>(
                                new RotationImpl(angle));
                    }
                    break;
                default:
                    std::cout << "Can't handle unknown PID command";
                    continue;
            }

            bool commandDone = false;

            while (!commandDone && !stopVar)
            {
                commandDone = command->doCommand(this);
                
                std::this_thread::sleep_until(t + pidDelay);
                t += pidDelay;
            }
            
            if (!stopVar)
            {
                cn::NotificationMessage msg;
                msg.set_type(cn::NotificationMessage::CMD_EXECUTION);
                auto cmdEx = msg.mutable_cmd_execution_result();
                cmdEx->set_command_index(cPtr->getCommandId());
                cmdEx->set_execution_result((commandDone) ?
                    cn::CmdExecutionResult::SUCCESS :
                    cn::CmdExecutionResult::FAIL);

                if (!commandDone)
                {
                    cmdEx->set_reason("Failed to execute command");
                }
                
                AbstractSender<cn::NotificationMessage>::sendMessage(
                    msg);
            }
        }
        else
        {
            std::this_thread::sleep_until(t + commandDelay);
            t += commandDelay;
        }
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
    
    std::cout << "PID controller started" << std::endl;
}

void PIDController::stopController()
{    
    std::cout << "Try to stop PID controller" << std::endl;
    
    if (!stopVar)
    {
        stopVar = true;

        if (controllerThread)
        {
            controllerThread->join();
            controllerThread.reset();
            imuReader = nullptr;

            std::cout << "PID controller stoped" << std::endl;
        }
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
    
    AbstractSender<cc::CommandMessage>::sendMessage(commandMessage);
}

void PIDController::doRotation(float angle, float& leftFactor, 
        float& rightFactor, int& rotDirection)
{
    float tmpLeft, tmpRight;
    int tmpDirection;
    float absAngle = fabs(angle);
    
    if (absAngle > 40.0f)
    {
        tmpLeft = tmpRight = 1.0f;
    }
    else if (absAngle <= 40.0f && absAngle > 20.0f)
    {
        tmpLeft = tmpRight = 0.8f;
    }
    else
    {
        tmpLeft = tmpRight = 0.6f;
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
    
    AbstractSender<cc::CommandMessage>::sendMessage(commandMessage);
}

void PIDController::putRotation(uint64_t cmdId, float angle)
{
    std::lock_guard<std::mutex> l(queueMutex);
    std::shared_ptr<IPIDCommand> c = std::shared_ptr<IPIDCommand>(
            new RotateCommand(cmdId, angle));
    
    commandsQueue.push(c);
}

std::shared_ptr<IPIDCommand> PIDController::getCommand()
{
    std::lock_guard<std::mutex> l(queueMutex);
    if (commandsQueue.empty())
    {
        return std::shared_ptr<IPIDCommand>();
    }
    
    std::shared_ptr<IPIDCommand> c = commandsQueue.front();
    commandsQueue.pop();
    
    return c;
}

PIDController::RotationImpl::RotationImpl(float angle)
: rotLeftFactor(INFINITY)
, rotRightFactor(INFINITY)
, rotDirection(0)
{
    Euler2Quaternion(0, 0, angle * M_PI / 180, &targetQ);
}

PIDController::RotationImpl::~RotationImpl()
{
    
}

bool PIDController::RotationImpl::doCommand(PIDController *owner)
{
    bool result = false;
    auto angles = owner->imuReader->getCurrentAngles();

    QUATERNION qIMU, qIMUConj, qRot;
    Euler2Quaternion(0, 0, angles.yaw * M_PI / 180, &qIMU);
    QuaternionConj(&qIMU, &qIMUConj);
    QuaternionProd(&targetQ, &qIMUConj, &qRot);

    float rollRot, pitchRot, yawRot;
    Quaternion2Euler(&qRot, &rollRot, &pitchRot, &yawRot);

    yawRot *= 180 / M_PI;

    if (fabs(yawRot) < 2)
    {
        result = true;
        owner->stopRotation();
    }
    else
    {
        owner->doRotation(yawRot, rotLeftFactor, rotRightFactor,
                rotDirection);
    }
    
    return result;
}
