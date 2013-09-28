#include "pincontroller/pwm.hpp"
#include "details/pwm_impl.hpp"
#include "details/pwm_factory.hpp"

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
        dispose();
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

    uint32_t PWM::getDuty() const
    {
        auto sp = check();
        return sp->getDuty();                
    }
    
    uint32_t PWM::getPeriod() const
    {
        auto sp = check();
        return sp->getPeriod();                
    }
    
    PWM_CHANNEL PWM::getPWMPin() const
    {
        auto sp = check();
        return sp->getPWMPin();                
    }
    
    void PWM::dispose()
    {
        if (implPtr.expired())
        {
            return;
        }
        
        if (implPtr.use_count() == 1)
        {
            auto instance = PWMFactory::getInstance();
            
            if (instance)
            {
                instance->releasePWM(getPWMPin());
            }
        }
        
        implPtr.reset();
    }
    
    void PWM::releasePwmChannel()
    {
        auto instance = PWMFactory::getInstance();

        if (!instance)
        {
            PC_EXCEPTION(InternalErrorException, 
                    "Can't get PWM factory instance");
        }
        
        instance->releasePWM(getPWMPin());
        implPtr.reset();
    }
}
