/* 
 * File:   DriveController.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 23, 2013, 12:37 AM
 */

#ifndef DRIVECONTROLLER_HPP
#define	DRIVECONTROLLER_HPP

#include <memory>
#include <mutex>

#include "pincontroller/pincontroller.hpp"

class DriveController 
{
private:
    DriveController();
    
public:
    enum class DriveGroup
    {
        GROUP_A,
        GROUP_B
    };
    
    enum class MoveDirection
    {
        MOVE_FORWARD,
        MOVE_BACKWARD
    };
    
public:
    virtual ~DriveController();
    
    static DriveController* getInstance();
    
    void runDriveGroup(DriveGroup group, MoveDirection direction,
        float drivePower);
    void stopDrives();
    
private:
    void setGroupDirection(DriveGroup group, bool isForward);

private:
    static std::unique_ptr<DriveController> instance;
    
    std::unique_ptr<pc::PWM> pwmA;
    std::unique_ptr<pc::PWM> pwmB;
    std::unique_ptr<pc::GPIOPin> gpioDirectionA;
    std::unique_ptr<pc::GPIOPin> gpioDirectionB;
    
    static std::mutex controllerMutex;
};

#endif	/* DRIVECONTROLLER_HPP */
