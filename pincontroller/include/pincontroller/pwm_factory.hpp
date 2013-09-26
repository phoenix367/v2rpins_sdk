/* 
 * File:   pwm_factory.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 21:39
 */

#ifndef PWM_FACTORY_HPP
#define	PWM_FACTORY_HPP

#include "global.hpp"
#include "pwm_impl.hpp"

#include <vector>
#include <unordered_map>

namespace pc
{
    class PWMFactory
    {
    private:
        PWMFactory();
        
    public:
        virtual ~PWMFactory();
        
        static PWMFactory* getInstance();
        
        std::shared_ptr<PWMImpl> createPWM(PWM_OUTPUT po);
        
    private:
        static std::unique_ptr<PWMFactory> instance;
        
        std::unordered_map<int, std::shared_ptr<PWMImpl> > pwmMap;
    };
}

#endif	/* PWM_FACTORY_HPP */
