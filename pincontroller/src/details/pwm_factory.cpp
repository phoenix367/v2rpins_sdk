#include <memory>

#include "pwm_factory.hpp"
#include "pincontroller/exceptions.hpp"
#include "gpio_manager.hpp"

namespace pc
{
    std::unique_ptr<PWMFactory> PWMFactory::instance;
    
    const std::unordered_map<PWM_CHANNEL, Contact> PWMFactory::contactMap =
    {
        { PWM_CHANNEL::PWM_0, Contact::con1_43 },
        { PWM_CHANNEL::PWM_1, Contact::con1_44 },
        { PWM_CHANNEL::PWM_2, Contact::con1_15 },
        { PWM_CHANNEL::PWM_3, Contact::con1_42 },
    };
    
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
        
        auto gpioInstance = GPIOManager::getInstance();
        if (!gpioInstance)
        {
            PC_EXCEPTION(InternalErrorException, "Instance of GPIO manager "
                    "object is NULL.");
        }
                
        if (po < PWM_CHANNEL::PWM_0 || po >= PWM_CHANNEL::PWM_MAX)
        {
            PC_EXCEPTION(IncorrectParamException, "Invalid PWM pin index.");
        }
        
        gpioInstance->aquire(contactMap.at(po));

        std::shared_ptr<PWMImpl> sp;
        
        try
        {
            sp = std::shared_ptr<PWMImpl>(new PWMImpl(po));
        }
        catch (Exception& e)
        {
            gpioInstance->release(contactMap.at(po));
            throw e;
        }
        
        pwmMap[po] = sp;
        return sp;
    }
    
    void PWMFactory::releasePWM(PWM_CHANNEL po)
    {
        auto gpioInstance = GPIOManager::getInstance();
        if (!gpioInstance)
        {
            PC_EXCEPTION(InternalErrorException, "Instance of GPIO manager "
                    "object is NULL.");
        }

        gpioInstance->release(contactMap.at(po));
        pwmMap.erase(po);
    }
}
