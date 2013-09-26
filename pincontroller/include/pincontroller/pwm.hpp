/* 
 * File:   pwm.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 22:35
 */

#ifndef PWM_HPP
#define	PWM_HPP

#include "global.hpp"
#include "reference.hpp"

#include <cstdint>

namespace pc
{
    class PWMImpl;
    
    class PWM : public Reference<PWMImpl>
    {
    public:
        PWM();
        PWM(PWM_OUTPUT output);
        virtual ~PWM();
        
        void setPulseParams(uint32_t duty, uint32_t period);
        void init();
        void stop();
        
        uint32_t getDuty();
        uint32_t getPeriod();
        PWM_OUTPUT getPWMPin();
                
    private:
    };
}

#endif	/* PWM_HPP */

