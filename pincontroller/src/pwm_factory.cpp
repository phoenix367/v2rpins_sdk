#include <memory>

#include "pincontroller/pwm_factory.hpp"
#include "pincontroller/exceptions.hpp"

namespace pc
{
    std::unique_ptr<PWMFactory> PWMFactory::instance;
    
    PWMFactory::PWMFactory()
    {
        
    }
    
    PWMFactory::~PWMFactory()
    {
        
    }
    
    PWMFactory* PWMFactory::getInstance()
    {
        if (!instance)
        {
            instance = std::unique_ptr<PWMFactory>(new PWMFactory());
        }
        
        return instance.get();
    }
    
    std::shared_ptr<PWMImpl> PWMFactory::createPWM(PWM_CHANNEL po)
    {
        if (pwmMap.find(po) != pwmMap.end())
        {
            PC_EXCEPTION(PinLockedException, "Selected PWM pin already "
                    "locked.");
        }
        
        if (po < PWM_CHANNEL::PWM_0 || po >= PWM_CHANNEL::PWM_MAX)
        {
            PC_EXCEPTION(IncorrectParamException, "Invalid PWM pin index.");
        }
        
        auto sp = std::shared_ptr<PWMImpl>(new PWMImpl(po));
        pwmMap[po] = sp;
        
        return sp;
    }
}
