/* 
 * File:   gpio_pin_impl.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 13, 2013, 12:19 AM
 */

#ifndef GPIO_PIN_IMPL_HPP
#define	GPIO_PIN_IMPL_HPP

#include "pincontroller/global.hpp"
#include "dev_helper.hpp"

namespace pc
{
    class GPIOPinImpl
    {
    public:
        GPIOPinImpl(GPIO_PIN p, GPIO_DIRECTION d);
        virtual ~GPIOPinImpl();
        
    private:
        GPIO_PIN pin;
        GPIO_DIRECTION direction;
        
        DevHelper devHelper;
    };
}

#endif	/* GPIO_PIN_IMPL_HPP */
