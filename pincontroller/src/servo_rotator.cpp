#include "pincontroller/servo_rotator.hpp"
#include "pincontroller/exceptions.hpp"

namespace pc
{
    const uint32_t ServoRotator::DEFAULT_IMP_PERIOD = 20000;
    const uint32_t ServoRotator::DEFAULT_MIN_DUTY = 700;
    const uint32_t ServoRotator::DEFAULT_MAX_DUTY = 2200;
    const uint16_t ServoRotator::DEFAULT_MAX_ANGLE = 180;
    
    ServoRotator::ServoRotator(PWM& extPwm)
    : pwm(extPwm)
    {
        init();
    }
    
    ServoRotator::ServoRotator(PWM_CHANNEL pwmChannel)
    : pwm(pwmChannel)
    {
        init();
    }
    
    ServoRotator::~ServoRotator()
    {
        
    }
    
    void ServoRotator::init()
    {
        impPeriod = DEFAULT_IMP_PERIOD;
        minDuty = DEFAULT_MIN_DUTY;
        maxDuty = DEFAULT_MAX_DUTY;
        maxAngle = DEFAULT_MAX_ANGLE;
    }
    
    uint32_t ServoRotator::getImpulsePeriod() const
    {
        return impPeriod;
    }
    
    void ServoRotator::setImpulsePeriod(uint32_t newPeriod)
    {
        impPeriod = newPeriod;
    }
    
    uint32_t ServoRotator::getMinDuty() const
    {
        return minDuty;
    }
    
    void ServoRotator::setMinDuty(uint32_t newMinDuty)
    {
        minDuty = newMinDuty;
    }
    
    uint32_t ServoRotator::getMaxDuty() const
    {
        return maxDuty;
    }
    
    void ServoRotator::setMaxDuty(uint32_t newMaxDuty)
    {
        maxDuty = newMaxDuty;
    }
    
    void ServoRotator::rotate(uint16_t angle)
    {
        if (angle > maxAngle)
        {
            PC_EXCEPTION(OutOfRangeException, "Angle value is too big.");
        }
        
        if (maxDuty <= minDuty)
        {
            PC_EXCEPTION(IncorrectParamException, 
                    "Maximal duty should be greater than minimal duty.");
        }
        
        uint32_t duty = minDuty + (angle * (maxDuty - minDuty)) / 
                maxAngle;
        
        pwm.setPulseParams(duty, impPeriod);
        pwm.init();
    }
    
    void ServoRotator::setMaxAngle(uint16_t newMaxAngle)
    {
        maxAngle = newMaxAngle;
    }
    
    uint16_t ServoRotator::getMaxAngle()
    {
        return maxAngle;
    }
}
