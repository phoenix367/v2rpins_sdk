/* 
 * File:   gpio_manager.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 8, 2013, 10:06 PM
 */

#ifndef GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include <unordered_map>
#include <memory>

#include "hash.hpp"

namespace pc
{   
    class GPIOManager
    {
    private:
        GPIOManager();
        
    public:
        virtual ~GPIOManager();
        
        static GPIOManager* getInstance();
        
        void aquire(Contact c);
        void release(Contact c);
        
    private:
        std::unordered_map<Contact, ContactState> contacts;        
        static const std::unordered_map<GPIO_PIN, Contact> gpioMap;
        
        static std::unique_ptr<GPIOManager> instance;
    };
}

#endif	/* GPIO_MANAGER_HPP */

