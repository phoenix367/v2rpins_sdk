/* 
 * File:   PIDController.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 21, 2014, 12:36 AM
 */

#ifndef PIDCONTROLLER_HPP
#define	PIDCONTROLLER_HPP

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>

#include "CommandSender.hpp"
#include "IMUReader.hpp"
#include "PIDCommands.hpp"
#include "madgwik_ahrs.h"
#include "ConfigManager.hpp"

class PIDController : public CommandSender, public NotifySender
{
private:
    PIDController();
    
    enum CommandState
    {
        inProgress,
        success,
        fail
    };

    class CommandImpl
    {
    public:
        typedef std::chrono::high_resolution_clock::time_point tp;
        
        CommandImpl(const tp& t)
        : baseTime(t)
        {}
        
        virtual ~CommandImpl() {}
        
        virtual CommandState doCommand(const tp& t,
                PIDController *owner) = 0;
    
    protected:
        tp baseTime;
    };
    
    class RotationImpl : public CommandImpl
    {
    public:
        RotationImpl(const tp& t, float angle);
        virtual ~RotationImpl();
        
        virtual CommandState doCommand(const tp& t, PIDController *owner);
        
    private:
        QUATERNION targetQ;
        float rotLeftFactor;
        float rotRightFactor;
        int rotDirection;
        
        float previousError;
        float integral;
        int stopCriteria;        
        RotationPIDConstants pidConstants;
    };
    
    class WaitImpl : public CommandImpl
    {
    public:
        WaitImpl(const tp& t, float d);
        virtual ~WaitImpl();
        
        virtual CommandState doCommand(const tp& t, PIDController *);
        
    private:
        std::chrono::milliseconds duration;
    };
        
    class MoveTimeImpl : public CommandImpl
    {
    public:
        MoveTimeImpl(const tp& t, float d, bool dir,
                const EulerAngles& angles);
        virtual ~MoveTimeImpl();
        
        virtual CommandState doCommand(const tp& t, PIDController *);
        
    private:
        std::chrono::milliseconds duration;
        bool direction;
        bool drivesStarted;
        QUATERNION targetQ;
        float previousError;
        float integral;
    };

public:
    virtual ~PIDController();
    
    static PIDController* getInstance();
    
    void startController();
    void stopController();
    
    void setIMUReader(IMUReader* pReader);
    void putRotation(uint64_t cmdId, float angle);
    void putWait(uint64_t cmdId, float duration);
    void putMoveTime(uint64_t cmdId, float duration, bool direction);
    
private:
    void run();
    void stopRotation();
    void doRotation(float leftFactor, float rightFactor, int rotDirection);
    void doMove(bool direction, float leftFactor = 1.0f, 
            float rightFactor = 1.0f);
    std::shared_ptr<IPIDCommand> getCommand();
    
private:
    static std::unique_ptr<PIDController> instance;
    
    std::atomic<bool> stopVar;
    std::unique_ptr<std::thread> controllerThread;
    IMUReader *imuReader;
    std::queue<std::shared_ptr<IPIDCommand> > commandsQueue;
    std::mutex queueMutex;
    std::chrono::milliseconds commandSampleTime;
    std::chrono::milliseconds waitSampleTime;
};

#endif	/* PIDCONTROLLER_HPP */
