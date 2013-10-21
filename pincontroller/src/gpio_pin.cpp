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
        
    }
}
