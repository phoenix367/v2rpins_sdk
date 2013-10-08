/* 
 * File:   gpio_manager.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 8, 2013, 10:06 PM
 */

#ifndef GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include <unordered_map>

#include "hash.hpp"

namespace pc
{   
    class GPIOManager
    {
    public:
        
    private:
        static std::unordered_map<Contact, ContactState> contacts;        
        static const std::unordered_map<GPIO_PIN, Contact> gpioMap;
    };
}

#endif	/* GPIO_MANAGER_HPP */

