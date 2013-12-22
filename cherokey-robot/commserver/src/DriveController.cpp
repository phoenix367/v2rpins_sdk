/* 
 * File:   DriveController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 23, 2013, 12:37 AM
 */

#include <boost/math/special_functions/round.hpp>

#include "DriveController.hpp"

#define PWM_FREQUENCY 400                           // PWM frequency is 400 Hz
#define PWM_PERIOD (1000000 / PWM_FREQUENCY)        // PWM period in 
                                                    // microseconds

std::unique_ptr<DriveController> DriveController::instance;

DriveController::DriveController()
: pwmA(new pc::PWM(pc::PWM_CHANNEL::PWM_0))
, pwmB(new pc::PWM(pc::PWM_CHANNEL::PWM_1))
{

}

DriveController::~DriveController() 
{
}

DriveController* DriveController::getInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<DriveController>(new DriveController());
    }
    
    return instance.get();
}

void DriveController::runDriveGroup(DriveGroup group, MoveDirection direction,
        float drivePower)
{
    if (drivePower > 1.0f || drivePower < 0.0f)
    {
        
    }
    
    uint32_t period = (uint32_t) boost::math::round(drivePower * PWM_PERIOD);
    
    switch (direction)
    {
        case MoveDirection::MOVE_FORWARD:
            break;
        case MoveDirection::MOVE_BACKWARD:
            break;
    }
}
