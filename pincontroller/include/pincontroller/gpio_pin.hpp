/* 
 * File:   GPIOPin.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 13, 2013, 12:10 AM
 */

#ifndef GPIOPIN_HPP
#define	GPIOPIN_HPP

#include "reference.hpp"
#include "global.hpp"

namespace pc
{
    class GPIOPinImpl;
    
    /**
     * 
     */
    class GPIOPin : public Reference<GPIOPinImpl>
    {
    public:
        /**
         * 
         */
        GPIOPin();
        
        /**
         * 
         * @param p
         * @param d
         */
        GPIOPin(GPIO_PIN p, GPIO_DIRECTION d, 
            GPIO_LOGIC_LEVEL ll = GPIO_LOGIC_LEVEL::low);
        
        /**
         * 
         */
        virtual ~GPIOPin();
    };
}

#endif	/* GPIOPIN_HPP */
