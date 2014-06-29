#include "gpio_pin_impl.hpp"
#include "pincontroller/exceptions.hpp"
#include "dev_helper.hpp"

namespace pc
{
    const std::string GPIOPinImpl::BASE_GPIO_FOLDER = "/sys/class/gpio/";
    const std::string GPIOPinImpl::EXPORT_FILE = "export";
    const std::string GPIOPinImpl::UNEXPORT_FILE = "unexport";
    const std::string GPIOPinImpl::DIRECTION_FILE = "direction";
    const std::string GPIOPinImpl::VALUE_FILE = "value";
    
    const int GPIOPinImpl::GPIO_COUNT = 104;
    
    GPIOPinImpl::GPIOPinImpl(GPIO_PIN p, Contact con, GPIO_DIRECTION d,
            GPIO_LOGIC_LEVEL ll)
    : pin(p)
    , direction(d)
    {
        devHelper = std::unique_ptr<DevHelper>(new DevHelper(
                DevHelper::GPIO_DEVICE));
        std::ostringstream stream;
        
        if (direction == GPIO_DIRECTION::input)
        {
            stream << "set gpio " << (int) p << " input";
        }
        else
        {
            stream << "set gpio " << (int) p << " output " << 
                    (int) ll;
        }
        
        devHelper->sendCommand(stream.str());
    }
    
    GPIOPinImpl::~GPIOPinImpl()
    {
        try
        {
            devHelper->close();
        }
        catch (Exception&)
        {
            // Ignore exception
        }
    }

    void GPIOPinImpl::setValue(GPIO_LOGIC_LEVEL ll)
    {
        if (direction == GPIO_DIRECTION::input)
        {
            PC_EXCEPTION(IncorrectParamException, 
                    "Can't set value for GPIO pin configured as input");
        }
        
        std::ostringstream stream;
        stream << "set gpio " << (int) pin << " output " << (int) ll;
        
        devHelper->sendCommand(stream.str());
        logicLevel = ll;
    }
    
    GPIO_LOGIC_LEVEL GPIOPinImpl::getValue() const
    {
        if (direction == GPIO_DIRECTION::output)
        {
            return logicLevel;
        }
        else
        {
            char gpioBuffer[GPIO_COUNT];
            devHelper->readData(gpioBuffer, GPIO_COUNT);
            
            char gpioState = gpioBuffer[(int) pin];
            
            return (gpioState == '0') ? GPIO_LOGIC_LEVEL::low :
                GPIO_LOGIC_LEVEL::high;
        }
    }

    GPIO_PIN GPIOPinImpl::getPinIndex()
    {
        return pin;
    }
}
