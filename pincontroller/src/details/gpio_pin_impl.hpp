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
#include "internal_enums.hpp"

#include <memory>

namespace pc
{
    class GPIOPinImpl
    {
    public:
        GPIOPinImpl(GPIO_PIN p, Contact con, GPIO_DIRECTION d,
                GPIO_LOGIC_LEVEL ll);
        virtual ~GPIOPinImpl();
        
        void setValue(GPIO_LOGIC_LEVEL ll);
        GPIO_LOGIC_LEVEL getValue() const;
        
    private:
        void exportGpio(int gpioNumber);
        void unExportGpio(int gpioNumber);
        
    private:
        static const std::string BASE_GPIO_FOLDER;
        static const std::string EXPORT_FILE;
        static const std::string UNEXPORT_FILE;
        static const std::string DIRECTION_FILE;
        static const std::string VALUE_FILE;
        
    private:
        GPIO_PIN pin;
        GPIO_DIRECTION direction;
        GPIO_LOGIC_LEVEL logicLevel;
        std::string baseGpioPinFolder;
        
        std::unique_ptr<DevHelper> devHelper;
    };
}

#endif	/* GPIO_PIN_IMPL_HPP */
