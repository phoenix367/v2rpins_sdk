/* 
 * File:   DriveController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on December 23, 2013, 12:37 AM
 */

#include <boost/math/special_functions/round.hpp>

#include "DriveController.hpp"
#include "Exceptions.hpp"
#include "ConfigManager.hpp"

#define PWM_FREQUENCY 400                           // PWM frequency is 400 Hz
#define PWM_PERIOD (1000000 / PWM_FREQUENCY)        // PWM period in 
                                                    // microseconds

std::unique_ptr<DriveController> DriveController::instance;

DriveController::DriveController()
: pwmA(new pc::PWM(pc::PWM_CHANNEL::PWM_0))
, pwmB(new pc::PWM(pc::PWM_CHANNEL::PWM_1))
{
    auto instance = ConfigManager::getInstance();
    
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Configuration manager "
            "instance is null.");
    }
    
    auto pinConfig = instance->getPinsInfo();
    
    gpioDirectionA = std::unique_ptr<pc::GPIOPin>(
            new pc::GPIOPin(pinConfig.driveADirectionPin, 
                pc::GPIO_DIRECTION::output));
    gpioDirectionB = std::unique_ptr<pc::GPIOPin>(
            new pc::GPIOPin(pinConfig.driveBDirectionPin, 
                pc::GPIO_DIRECTION::output));
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
        COMM_EXCEPTION(OutOfRangeException, "Drive power value is "
                "out of range");
    }
    
    uint32_t period = (uint32_t) boost::math::round(drivePower * PWM_PERIOD);
    
    switch (direction)
    {
        case MoveDirection::MOVE_FORWARD:
            setGroupDirection(group, true);
            break;
        case MoveDirection::MOVE_BACKWARD:
            setGroupDirection(group, false);
            break;
    }

    switch (group)
    {
        case DriveGroup::GROUP_A:
            pwmA->setPulseParams(period, PWM_PERIOD);
            pwmA->init();
            break;
        case DriveGroup::GROUP_B:
            pwmB->setPulseParams(period, PWM_PERIOD);
            pwmB->init();
            break;
    }
}

void DriveController::setGroupDirection(DriveGroup group, bool isForward)
{
    switch (group)
    {
        case DriveGroup::GROUP_A:
            gpioDirectionA->setLogicalLevel((isForward) ? 
                pc::GPIO_LOGIC_LEVEL::low : pc::GPIO_LOGIC_LEVEL::high);
            break;
        case DriveGroup::GROUP_B:
            gpioDirectionB->setLogicalLevel((isForward) ? 
                pc::GPIO_LOGIC_LEVEL::low : pc::GPIO_LOGIC_LEVEL::high);
            break;
    }
}

void DriveController::stopDrives()
{
    pwmA->setPulseParams(0, PWM_PERIOD);
    pwmA->init();
    pwmB->setPulseParams(0, PWM_PERIOD);
    pwmB->init();
}
