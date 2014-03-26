/* 
 * File:   IndicatorController.cpp
 * Author: Ivan Gubochkin
 * 
 * Created on March 9, 2014, 11:23 PM
 */

#include "IndicatorController.hpp"
#include "ConfigManager.hpp"
#include "Exceptions.hpp"

#include "notifications.pb.h"

namespace cn = cherokey::notifications;

std::unique_ptr<IndicatorController> IndicatorController::instance;

IndicatorController::IndicatorController() 
{
    auto instance = ConfigManager::getInstance();
    
    if (!instance)
    {
        COMM_EXCEPTION(NullPointerException, "Pointer to instance of "
                "configuration manager is null.");
    }

    auto pins = instance->getPinsInfo();
    
    calibrationIndPin = std::unique_ptr<pc::GPIOPin>(
            new pc::GPIOPin(pins.calibIndPin, 
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
        
        cn::NotificationMessage msg;
        msg.set_type(cn::NotificationMessage::CALIBRATION_STATE);
        auto notifyMsg = msg.mutable_calibration_state();
        notifyMsg->set_state((bState) ? 
            cn::CalibrationState::CALIBRATION_BEGIN :
            cn::CalibrationState::CALIBRATION_DONE);
        sendMessage(msg);
    }
}
