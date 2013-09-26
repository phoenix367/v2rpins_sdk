/* 
 * File:   dev_helper.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 19:45
 */

#ifndef DEV_HELPER_HPP
#define	DEV_HELPER_HPP

#include <string>

namespace pc
{
    class DevHelper
    {
    public:
        static const std::string PINS_DEVICE;
        static const int INV_FILE_HANDLE;
        static const std::string DEV_PREFIX;
        
    public:
        DevHelper(const std::string& devName);
        virtual ~DevHelper();
        
        void open(const std::string& devName);
        void close();
        bool isOpened();
        void sendCommand(const std::string& cmd);
        
        static void doCommand(const std::string& devName,
                const std::string& command);
        
    private:
        int devFileHandle;
    };
}

#endif	/* DEV_HELPER_HPP */
