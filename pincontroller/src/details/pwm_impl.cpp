#include "pwm_impl.hpp"
#include "dev_helper.hpp"
#include "pincontroller/exceptions.hpp"

#include <sstream>

namespace pc
{
    uint64_t PWMImpl::PWM_GENERATOR_FRQ = 24000000ULL;
    
    const PWMImpl::ConnectionMap PWMImpl::conMap =
    {
        {PWM_CHANNEL::PWM_0, "con 43"}, {PWM_CHANNEL::PWM_1, "con 44"}, 
        {PWM_CHANNEL::PWM_2, "con 16"}, {PWM_CHANNEL::PWM_3, "con 42"}
    };
    
    const std::string PWMImpl::PWM_PREFIX = "pwm";
    
    PWMImpl::PWMImpl(PWM_CHANNEL po)
    : pwmPin(po)
    , ticDuty(0ULL)
    , ticPeriod(0ULL)
    , devHelper(DevHelper::PINS_DEVICE)
    {
        std::string con = conMap.at(po);
        std::ostringstream stream;
        
        stream << "set " << con << " " << PWM_PREFIX << 
                (int) pwmPin;
        devHelper.sendCommand(stream.str());
    }
    
    PWMImpl::~PWMImpl() noexcept
    {
        try
        {
            stop();
            devHelper.close();
        }
        catch (Exception&)
        {
            // Ignore exception
        }
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
    
    void PWMImpl::setPwmClock(PWM_CHANNEL pin, uint64_t duty, 
            uint64_t period)
    {
        std::ostringstream stream;
        
        // We subtract 1 from period value because PWM
        // controller automatically add it
        uint64_t hwPeriod = (period > 0) ? period - 1 : 0;
        stream << "set " << PWM_PREFIX << " " << (int) pin << " " <<
                duty << " " << hwPeriod;
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
    
    PWM_CHANNEL PWMImpl::getPWMPin()
    {
        return pwmPin;
    }
    
    uint32_t PWMImpl::tic2time(uint64_t tic)
    {
        return (tic / 1000000ULL) * PWM_GENERATOR_FRQ;
    }
}
