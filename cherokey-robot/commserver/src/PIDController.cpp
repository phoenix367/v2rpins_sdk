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
#include <fstream>

#define PID_DIAGNOSTIC          0

namespace cc = cherokey::common;
namespace cn = cherokey::notifications;

std::unique_ptr<PIDController> PIDController::instance;

#if PID_DIAGNOSTIC
std::ofstream pidStream;
#endif

PIDController::PIDController() 
: stopVar(true)
, imuReader(nullptr)
{
    auto instance = ConfigManager::getInstance();
    
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Configuration mamager "
                "instance is null.");
    }
    
    auto info = instance->getCommonPIDInfo();
    commandSampleTime = std::chrono::milliseconds(info.commandSampleTime);
    waitSampleTime = std::chrono::milliseconds(info.waitSampleTime);
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
                                new RotationImpl(t, angle));
                    }
                    break;
                case CommandType::wait:
                    {
                        auto cmd = cPtr->commandInstance<WaitCommand>();
                        float d = cmd->getDuration();

                        std::cout << 
                                "Start executing wait command, duration:" <<
                                d << std::endl;
                        
                        command = std::unique_ptr<CommandImpl>(
                                new WaitImpl(t, d));
                    }
                    break;
                case CommandType::moveTime:
                    {
                        auto cmd = cPtr->commandInstance<MoveTimeCommand>();
                        float d = cmd->getDuration();
                        bool dir = cmd->getDirection();

                        std::cout << 
                                "Start executing move command, duration:" <<
                                d << ", direction: " << dir << std::endl;
                        
                        command = std::unique_ptr<CommandImpl>(
                                new MoveTimeImpl(t, d, dir));
                    }
                    break;
                default:
                    std::cout << "Can't handle unknown PID command";
                    continue;
            }

            CommandState commandState = inProgress;

            while (commandState == inProgress && !stopVar)
            {
                commandState = command->doCommand(t, this);
                
                std::this_thread::sleep_until(t + commandSampleTime);
                t += commandSampleTime;
            }
            
            if (!stopVar)
            {
                cn::NotificationMessage msg;
                msg.set_type(cn::NotificationMessage::CMD_EXECUTION);
                auto cmdEx = msg.mutable_cmd_execution_result();
                cmdEx->set_command_index(cPtr->getCommandId());
                cmdEx->set_execution_result((commandState == success) ?
                    cn::CmdExecutionResult::SUCCESS :
                    cn::CmdExecutionResult::FAIL);

                if (commandState == fail)
                {
                    cmdEx->set_reason("Failed to execute command");
                }
                
                AbstractSender<cn::NotificationMessage>::sendMessage(
                    msg);
            }
        }
        else
        {
            std::this_thread::sleep_until(t + waitSampleTime);
            t += waitSampleTime;
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

void PIDController::doRotation(float leftFactor, float rightFactor, 
        int rotDirection)
{
    auto driveInstance = DriveController::getInstance();

    if (!driveInstance)
    {
        COMM_EXCEPTION(NullPointerException, "Drive controller instance "
                "is null.");
    }

    DriveController::MoveDirection directionA, directionB;

    if (rotDirection > 0)
    {
        directionA = DriveController::MoveDirection::MOVE_FORWARD;
        directionB = DriveController::MoveDirection::MOVE_BACKWARD;
    }
    else
    {
        directionA = DriveController::MoveDirection::MOVE_BACKWARD;
        directionB = DriveController::MoveDirection::MOVE_FORWARD;
    }

    driveInstance->runDriveGroup(DriveController::DriveGroup::GROUP_A, 
            directionA, rightFactor);
    driveInstance->runDriveGroup(DriveController::DriveGroup::GROUP_B, 
            directionB, leftFactor);
}

void PIDController::doMove(bool direction)
{
    cc::CommandMessage commandMessage;
    commandMessage.set_type(cc::CommandMessage::MOVE);
    commandMessage.set_cookie(0);
    
    cc::MoveAction *moveCmd = commandMessage.mutable_move_action();
    cc::RunDriveGroup *groupA = moveCmd->mutable_run_group_a();
    
    if (direction)
    {
        groupA->set_direction(cc::RunDriveGroup::FORWARD);
    }
    else
    {
        groupA->set_direction(cc::RunDriveGroup::BACKWARD);
    }
    
    groupA->set_power(1.0f);

    cc::RunDriveGroup *groupB = moveCmd->mutable_run_group_b();

    if (direction)
    {
        groupB->set_direction(cc::RunDriveGroup::FORWARD);
    }
    else
    {
        groupB->set_direction(cc::RunDriveGroup::BACKWARD);
    }
    
    groupB->set_power(1.0f);
    
    AbstractSender<cc::CommandMessage>::sendMessage(commandMessage);
}

void PIDController::putRotation(uint64_t cmdId, float angle)
{
    std::lock_guard<std::mutex> l(queueMutex);
    std::shared_ptr<IPIDCommand> c = std::shared_ptr<IPIDCommand>(
            new RotateCommand(cmdId, angle));
    
    commandsQueue.push(c);
}

void PIDController::putWait(uint64_t cmdId, float duration)
{
    std::lock_guard<std::mutex> l(queueMutex);
    std::shared_ptr<IPIDCommand> c = std::shared_ptr<IPIDCommand>(
            new WaitCommand(cmdId, duration));
    
    commandsQueue.push(c);
}

void PIDController::putMoveTime(uint64_t cmdId, float duration, bool direction)
{
    std::lock_guard<std::mutex> l(queueMutex);
    std::shared_ptr<IPIDCommand> c = std::shared_ptr<IPIDCommand>(
            new MoveTimeCommand(cmdId, duration, direction));
    
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

PIDController::RotationImpl::RotationImpl(const tp& t,
        float angle)
: CommandImpl(t)
, rotLeftFactor(INFINITY)
, rotRightFactor(INFINITY)
, rotDirection(0)
, previousError(0)
, integral(0)
, stopCriteria(0)
{
    auto instance = ConfigManager::getInstance();
    
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Configuration manager instance "
                "is null");
    }
    
    pidConstants = instance->getRotationPIDInfo();
    Euler2Quaternion(0, 0, angle * M_PI / 180, &targetQ);
    
#if PID_DIAGNOSTIC
    pidStream.open("rotation_pid.txt", std::ofstream::out |
        std::ofstream::app);
#endif
}

PIDController::RotationImpl::~RotationImpl()
{
#if PID_DIAGNOSTIC
    pidStream.close();
#endif
}

PIDController::CommandState PIDController::RotationImpl::doCommand(
        const tp& t, PIDController *owner)
{
    CommandState result = inProgress;
    std::chrono::seconds commandDuration =
        std::chrono::duration_cast<std::chrono::seconds>(t - baseTime);
    float Kp = pidConstants.Kp;
    float Ki = pidConstants.Ki;
    float Kd = pidConstants.Kd;

    try
    {
        if (commandDuration.count() > pidConstants.rotationTimeout)
        {
            COMM_EXCEPTION(InternalError, "Execution is too long");
        }

        auto angles = owner->imuReader->getCurrentAngles();

        QUATERNION qIMU, qIMUConj, qRot;
        Euler2Quaternion(0, 0, angles.yaw * M_PI / 180, &qIMU);
        QuaternionConj(&qIMU, &qIMUConj);
        QuaternionProd(&targetQ, &qIMUConj, &qRot);

        float rollRot, pitchRot, yawRot;
        Quaternion2Euler(&qRot, &rollRot, &pitchRot, &yawRot);

        float error = yawRot * 180 / M_PI;
        
        integral = integral + Ki * error * 0.05f;
        
        if (fabs(integral) > 1.0f)
        {
            if (std::signbit(integral))
            {
                integral = -1.0f;
            }
            else
            {
                integral = 1.0f;
            }
        }
        
        float derivative = (error - previousError) / 0.05;
        float output = Kp * error + integral + Kd * derivative;
        
        if (std::signbit(output))
        {
            rotDirection = -1;
        }
        else
        {
            rotDirection = 1;
        }
        
        if (fabs(output) > 1.0f)
        {            
            if (std::signbit(output))
            {
                output = -1.0f;
            }
            else
            {
                output = 1.0f;
            }
        }
        
        auto deadZone = pidConstants.deadZone;
        
        if (output < deadZone.upperLimit && output > deadZone.lowerLimit)
        {
            if (error > 0)
            {
                integral = deadZone.upperLimit - Kp * error;
            }
            else
            {
                integral = deadZone.lowerLimit - Kp * error;
            }
        }
        
        std::cout << output << " " << error << " " << std::endl;
        
        rotLeftFactor = rotRightFactor = fabs(output);
        
        previousError = error;
        
#if PID_DIAGNOSTIC
        pidStream << angles.yaw << " " << error << " " << output << std::endl;
#endif
        
        if (fabs(error) < pidConstants.rotationPrecession)
        {
            if (!stopCriteria)
            {
                owner->stopRotation();
            }
            
            stopCriteria++;
            
            if (stopCriteria >= 5)
            {
                result = success;
            }
        }
        else
        {
            stopCriteria = 0;
            owner->doRotation(rotLeftFactor, rotRightFactor,
                    rotDirection);
        }
    }
    catch (Exception&)
    {
        result = fail;
        owner->stopRotation();
    }
    
    return result;
}

PIDController::WaitImpl::WaitImpl(const tp& t, float d)
: CommandImpl(t)
{
    duration = std::chrono::milliseconds((int64_t) (d * 1000.0f));
}

PIDController::WaitImpl::~WaitImpl()
{
    
}

PIDController::CommandState PIDController::WaitImpl::doCommand(const tp& t, 
        PIDController *)
{
    std::chrono::milliseconds commandDuration =
        std::chrono::duration_cast<std::chrono::milliseconds>(t - baseTime);
    
    if (commandDuration > duration)
    {
        return success;
    }
    
    return inProgress;
}

PIDController::MoveTimeImpl::MoveTimeImpl(const tp& t, float d, bool dir)
: CommandImpl(t)
, direction(dir)
, drivesStarted(false)
{
    duration = std::chrono::milliseconds((int64_t) (d * 1000.0f));
}

PIDController::MoveTimeImpl::~MoveTimeImpl()
{
    
}

PIDController::CommandState PIDController::MoveTimeImpl::doCommand(
    const tp& t, PIDController *owner)
{
    std::chrono::milliseconds commandDuration =
        std::chrono::duration_cast<std::chrono::milliseconds>(t - baseTime);

    try
    {
        if (commandDuration > duration)
        {
            owner->stopRotation();
            return success;
        }
        
        if (!drivesStarted)
        {
            owner->doMove(direction);
            drivesStarted = true;
        }
    }
    catch (Exception&)
    {
        owner->stopRotation();
        return fail;
    }
    
    return inProgress;
}
