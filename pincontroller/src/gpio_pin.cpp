#include "pincontroller/gpio_pin.hpp"
#include "details/gpio_pin_impl.hpp"
#include "details/gpio_manager.hpp"

namespace pc
{
    GPIOPin::GPIOPin()
    {
        
    }
    
    GPIOPin::GPIOPin(GPIO_PIN p, GPIO_DIRECTION d,
            GPIO_LOGIC_LEVEL ll)
    {
        auto instance = GPIOManager::getInstance();
        
        if (!instance)
        {
            PC_EXCEPTION(InternalErrorException,
                    "GPIO manager instance is NULL.");
        }
    }
    
    GPIOPin::~GPIOPin()
    {
        dispose();
    }

    void GPIOPin::dispose()
    {
        if (implPtr.expired())
        {
            return;
        }
        
        if (implPtr.use_count() == 1)
        {
            auto instance = GPIOManager::getInstance();
            
            if (instance)
            {
                instance->freeGpioPin(getPinIndex());
            }
        }
        
        implPtr.reset();
    }
    
    GPIO_PIN GPIOPin::getPinIndex() const
    {
        auto p = check();
        return p->getPinIndex();
    }
}
