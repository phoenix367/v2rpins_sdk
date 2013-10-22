#include "gpio_pin_impl.hpp"
#include "pincontroller/exceptions.hpp"

namespace pc
{
    const std::string GPIOPinImpl::BASE_GPIO_FOLDER = "/sys/class/gpio/";
    const std::string GPIOPinImpl::EXPORT_FILE = "export";
    const std::string GPIOPinImpl::UNEXPORT_FILE = "unexport";
    const std::string GPIOPinImpl::DIRECTION_FILE = "direction";
    const std::string GPIOPinImpl::VALUE_FILE = "value";
    
    GPIOPinImpl::GPIOPinImpl(GPIO_PIN p, Contact con, GPIO_DIRECTION d,
            GPIO_LOGIC_LEVEL ll)
    : pin(p)
    , direction(d)
    {
        exportGpio((int) pin);

        try
        {
            std::ostringstream stream;
            stream << BASE_GPIO_FOLDER << "gpio" << (int) pin << '/';
            baseGpioPinFolder = stream.str();

            switch (direction)
            {
                case GPIO_DIRECTION::input:
                    DevHelper::doCommand(baseGpioPinFolder + DIRECTION_FILE,
                            "in");
                    break;
                case GPIO_DIRECTION::output:
                    DevHelper::doCommand(baseGpioPinFolder + DIRECTION_FILE,
                            "out");
                    break;
            }

            devHelper = std::unique_ptr<DevHelper>(new DevHelper(
                    baseGpioPinFolder + VALUE_FILE));
            setValue(ll);
        }
        catch (Exception&)
        {
            devHelper.reset();
            unExportGpio((int) pin);
            throw;
        }
    }
    
    GPIOPinImpl::~GPIOPinImpl()
    {
        try
        {
            devHelper.reset();
            unExportGpio((int) pin);
        }
        catch (Exception&)
        {
            // Ignore exception
        }
    }

    void GPIOPinImpl::exportGpio(int gpioNumber)
    {
        std::ostringstream stream;
        
        stream << gpioNumber;
        DevHelper::doCommand(BASE_GPIO_FOLDER + EXPORT_FILE,
                stream.str());
    }
    
    void GPIOPinImpl::unExportGpio(int gpioNumber)
    {
        std::ostringstream stream;
        
        stream << gpioNumber;
        DevHelper::doCommand(BASE_GPIO_FOLDER + UNEXPORT_FILE,
                stream.str());
    }

    void GPIOPinImpl::setValue(GPIO_LOGIC_LEVEL ll)
    {
        switch (ll)
        {
        case GPIO_LOGIC_LEVEL::low:
            devHelper->sendCommand("0");
            break;
        case GPIO_LOGIC_LEVEL::high:
            devHelper->sendCommand("1");
            break;
        }
        
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
            return GPIO_LOGIC_LEVEL();
        }
    }
    
    GPIO_PIN GPIOPinImpl::getPinIndex()
    {
        return pin;
    }
}
