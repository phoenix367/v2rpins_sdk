#include "pincontroller/pwm_impl.hpp"
#include "pincontroller/dev_helper.hpp"
#include "pincontroller/exceptions.hpp"

#include <sstream>

namespace pc
{
    uint64_t PWMImpl::PWM_GENERATOR_FRQ = 24000000ULL;
    
    const std::unordered_map<int, std::string> PWMImpl::conMap =
    {
        {PWM_0, "con43"}, {PWM_1, "con44"}, {PWM_2, "con16"},
        {PWM_3, "con42"}
    };
    
    const std::string PWMImpl::PWM_PREFIX = "pwm";
    
    PWMImpl::PWMImpl(PWM_OUTPUT po)
    : pwmPin(po)
    , ticDuty(0ULL)
    , ticPeriod(0ULL)
    , devHelper(DevHelper::PINS_DEVICE)
    {
        std::string con = conMap.at(po);
        std::ostringstream stream;
        
        stream << "set " << con << " " << PWM_PREFIX << pwmPin;
        devHelper.sendCommand(stream.str());
    }
    
    PWMImpl::~PWMImpl()
    {
        devHelper.close();
    }
    
    void PWMImpl::setPulseParams(uint32_t duty, uint32_t period)
    {
        if (duty > period)
        {
            PC_EXCEPTION(IncorrectParamException, "Invalid PWM parameters.");
        }
        
        ticDuty = time2tic(duty);
        ticPeriod = time2tic(period);
    }
    
    uint64_t PWMImpl::time2tic(uint32_t t)
    {
        return (t * PWM_GENERATOR_FRQ) / 1000000ULL;
    }

    void PWMImpl::init()
    {
        setPwmClock(pwmPin, ticDuty, ticPeriod);
    }
    
    void PWMImpl::stop()
    {
        setPwmClock(pwmPin, 0, 0);
    }
    
    void PWMImpl::setPwmClock(PWM_OUTPUT pin, uint64_t duty, 
            uint64_t period)
    {
        std::ostringstream stream;
        
        stream << "set " << PWM_PREFIX << pin << " duty:" <<
                duty << " period:" << period;
        devHelper.sendCommand(stream.str());
    }

    uint32_t PWMImpl::getDuty()
    {
        return tic2time(ticDuty);
    }
    
    uint32_t PWMImpl::getPeriod()
    {
        return tic2time(ticPeriod);
    }
    
    PWM_OUTPUT PWMImpl::getPWMPin()
    {
        return pwmPin;
    }
    
    uint32_t PWMImpl::tic2time(uint64_t tic)
    {
        return (tic / 1000000ULL) * PWM_GENERATOR_FRQ;
    }
}
