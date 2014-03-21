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

#include "CommandSender.hpp"
#include "IMUReader.hpp"

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
    
private:
    void run();
    
private:
    static std::unique_ptr<PIDController> instance;
    
    std::atomic<bool> stopVar;
    std::unique_ptr<std::thread> controllerThread;
    IMUReader *imuReader;
};

#endif	/* PIDCONTROLLER_HPP */
