/* 
 * File:   pwm_impl.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 22:53
 */

#ifndef PWM_IMPL_HPP
#define	PWM_IMPL_HPP

#include "pincontroller/global.hpp"
#include "dev_helper.hpp"

#include <unordered_map>
#include <string>

namespace pc
{
    class PWMImpl
    {
    private:
        typedef std::unordered_map<PWM_CHANNEL, std::string> ConnectionMap;
    public:
        PWMImpl(PWM_CHANNEL po);
        virtual ~PWMImpl();
        
        void setPulseParams(uint32_t duty, uint32_t period);
        void init();
        void stop();
        
        uint32_t getDuty();
        uint32_t getPeriod();
        PWM_CHANNEL getPWMPin();
        
    private:
        uint64_t time2tic(uint32_t t);
        void setPwmClock(PWM_CHANNEL pin, uint64_t duty, uint64_t period);
        uint32_t tic2time(uint64_t tic);
        
    private:
        static uint64_t PWM_GENERATOR_FRQ;
        
        PWM_CHANNEL pwmPin;
        uint64_t ticDuty;
        uint64_t ticPeriod;
        DevHelper devHelper;
        
        const static ConnectionMap conMap;
        const static std::string PWM_PREFIX;
    };
}

#endif	/* PWM_IMPL_HPP */
