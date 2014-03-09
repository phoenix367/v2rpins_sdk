/* 
 * File:   IndicatorController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on March 9, 2014, 11:23 PM
 */

#include "IndicatorController.hpp"

std::unique_ptr<IndicatorController> IndicatorController::instance;

IndicatorController::IndicatorController() 
{
    calibrationIndPin = std::unique_ptr<pc::GPIOPin>(
            new pc::GPIOPin(pc::GPIO_PIN::gpio32, 
                pc::GPIO_DIRECTION::output));
}

IndicatorController::~IndicatorController() 
{
}

IndicatorController* IndicatorController::getInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<IndicatorController>(
                new IndicatorController());
    }
    
    return instance.get();
}

void IndicatorController::showCalibrationState(bool bState)
{
    if (calibrationIndPin)
    {
        calibrationIndPin->setLogicalLevel((bState) ? 
                pc::GPIO_LOGIC_LEVEL::high : pc::GPIO_LOGIC_LEVEL::low);
    }
}
