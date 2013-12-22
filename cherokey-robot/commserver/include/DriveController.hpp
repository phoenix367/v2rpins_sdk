/* 
 * File:   DriveController.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 23, 2013, 12:37 AM
 */

#ifndef DRIVECONTROLLER_HPP
#define	DRIVECONTROLLER_HPP

#include <memory>

#include "pincontroller/pwm.hpp"

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

private:
    static std::unique_ptr<DriveController> instance;
    
    std::unique_ptr<pc::PWM> pwmA;
    std::unique_ptr<pc::PWM> pwmB;
};

#endif	/* DRIVECONTROLLER_HPP */
