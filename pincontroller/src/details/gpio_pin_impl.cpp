#include "gpio_pin_impl.hpp"

namespace pc
{
    GPIOPinImpl::GPIOPinImpl(GPIO_PIN p, GPIO_DIRECTION d)
    : pin(p)
    , direction(d)
    , devHelper(DevHelper::PINS_DEVICE)
    {
        
    }
    
    GPIOPinImpl::~GPIOPinImpl()
    {
        
    }
}
