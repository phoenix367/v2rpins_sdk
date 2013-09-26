#include "pincontroller/pwm.hpp"
#include "pincontroller/pwm_impl.hpp"
#include "pincontroller/pwm_factory.hpp"

namespace pc
{
    PWM::PWM()
    {
        
    }
    
    PWM::PWM(PWM_CHANNEL output)
    {
        auto instance = PWMFactory::getInstance();
        if (!instance)
        {
            PC_EXCEPTION(InternalErrorException, 
                    "Can't get PWMFactory instance.");
        }
        
        implPtr = instance->createPWM(output);
    }
    
    PWM::~PWM()
    {
        
    }
    
    void PWM::setPulseParams(uint32_t duty, uint32_t period)
    {
        auto sp = check();
        sp->setPulseParams(duty, period);
    }

    void PWM::init()
    {
        auto sp = check();
        sp->init();        
    }
    
    void PWM::stop()
    {
        auto sp = check();
        sp->stop();        
    }

    uint32_t PWM::getDuty()
    {
        auto sp = check();
        return sp->getDuty();                
    }
    
    uint32_t PWM::getPeriod()
    {
        auto sp = check();
        return sp->getPeriod();                
    }
    
    PWM_CHANNEL PWM::getPWMPin()
    {
        auto sp = check();
        return sp->getPWMPin();                
    }
}
