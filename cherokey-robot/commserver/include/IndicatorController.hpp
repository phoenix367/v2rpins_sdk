/* 
 * File:   IndicatorController.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 9, 2014, 11:23 PM
 */

#ifndef INDICATORCONTROLLER_HPP
#define	INDICATORCONTROLLER_HPP

#include <memory>
#include "pincontroller/pincontroller.hpp"
#include "CommandSender.hpp"

class IndicatorController : public NotifySender
{
private:
    IndicatorController();
    
public:
    virtual ~IndicatorController();
    
    static IndicatorController* getInstance();
    void showCalibrationState(bool bState);
    
private:
    static std::unique_ptr<IndicatorController> instance;
    
    std::unique_ptr<pc::GPIOPin> calibrationIndPin;
};

#endif	/* INDICATORCONTROLLER_HPP */
