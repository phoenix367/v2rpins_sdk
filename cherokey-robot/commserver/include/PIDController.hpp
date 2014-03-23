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

class PIDController : public CommandSender
{
private:
    PIDController();
    
public:
    virtual ~PIDController();
    
    static PIDController* getInstance();
    
    void startController();
    void stopController();
    
    void setIMUReader(IMUReader* pReader);
    void putRotation(float angle);
    
private:
    void run();
    void stopRotation();
    void doRotation(float angle, float& leftFactor, float& rightFactor,
            int& rotDirection);
    void doRotation(float leftFactor, float rightFactor, int rotDirection);
    std::shared_ptr<IPIDCommand> getCommand();
    
private:
    static std::unique_ptr<PIDController> instance;
    
    std::atomic<bool> stopVar;
    std::unique_ptr<std::thread> controllerThread;
    IMUReader *imuReader;
    std::queue<std::shared_ptr<IPIDCommand> > commandsQueue;
    std::mutex queueMutex;
};

#endif	/* PIDCONTROLLER_HPP */
