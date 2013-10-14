#include "gpio_pin_impl.hpp"
#include "pincontroller/exceptions.hpp"

namespace pc
{
    GPIOPinImpl::GPIOPinImpl(GPIO_PIN p, GPIO_DIRECTION d)
    : pin(p)
    , direction(d)
    , devHelper(DevHelper::GPIO_DEVICE)
    {
        
    }
    
    GPIOPinImpl::~GPIOPinImpl()
    {
        try
        {
            devHelper.close();
        }
        catch (Exception&)
        {
            // Ignore exception
        }
    }
}
