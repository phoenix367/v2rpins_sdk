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
    
    GPIOPinImpl::GPIOPinImpl(GPIO_PIN p, Contact con, GPIO_DIRECTION d,
            GPIO_LOGIC_LEVEL ll)
    : pin(p)
    , direction(d)
    {
        std::ostringstream stream;
        
        if (direction == GPIO_DIRECTION::input)
        {
            stream << "set gpio" << (int) p << " input";
        }
        else
        {
            stream << "set gpio" << (int) p << " output:" << 
                    (int) ll;
        }
        
        DevHelper::doCommand(DevHelper::GPIO_DEVICE, stream.str());
        //exportGpio((int) pin);

        /*
        try
        {
            std::ostringstream stream;
            stream << BASE_GPIO_FOLDER << "gpio" << (int) pin << '/';
            baseGpioPinFolder = stream.str();

            switch (direction)
            {
                case GPIO_DIRECTION::input:
                    doCommand(baseGpioPinFolder + DIRECTION_FILE,
                            "in");
                    break;
                case GPIO_DIRECTION::output:
                    doCommand(baseGpioPinFolder + DIRECTION_FILE,
                            "out");
                    break;
            }

            fileHelper = std::unique_ptr<FileHelper>(new FileHelper(
                    baseGpioPinFolder + VALUE_FILE));
            setValue(ll);
        }
        catch (Exception&)
        {
            fileHelper.reset();
            unExportGpio((int) pin);
            throw;
        }
        */
    }
    
    GPIOPinImpl::~GPIOPinImpl()
    {
        try
        {
            //fileHelper.reset();
            //unExportGpio((int) pin);
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
        doCommand(BASE_GPIO_FOLDER + EXPORT_FILE,
                stream.str());
    }
    
    void GPIOPinImpl::unExportGpio(int gpioNumber)
    {
        std::ostringstream stream;
        
        stream << gpioNumber;
        doCommand(BASE_GPIO_FOLDER + UNEXPORT_FILE,
                stream.str());
    }

    void GPIOPinImpl::setValue(GPIO_LOGIC_LEVEL ll)
    {
        if (direction == GPIO_DIRECTION::input)
        {
            PC_EXCEPTION(IncorrectParamException, 
                    "Can't set value for GPIO pin configured as input");
        }
        
        std::ostringstream stream;
        stream << "set gpio" << (int) pin << " output:" << (int) ll;
        
        DevHelper::doCommand(DevHelper::GPIO_DEVICE, stream.str());
        logicLevel = ll;
        /*
        static const char lowValue = '0';
        static const char highValue = '1';
        
        switch (ll)
        {
        case GPIO_LOGIC_LEVEL::low:
            fileHelper->writeData(&lowValue, sizeof(char));
            break;
        case GPIO_LOGIC_LEVEL::high:
            fileHelper->writeData(&highValue, sizeof(char));
            break;
        }
        
        logicLevel = ll;
        */
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
    
    void GPIOPinImpl::doCommand(const std::string& file, 
            const std::string& command)
    {
        FileHelper helper(file, O_WRONLY);
        helper.writeData(command.c_str(), command.size());
        helper.close();
    }
}
