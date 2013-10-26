#include "dev_helper.hpp"
#include "pincontroller/exceptions.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

namespace pc
{
    const std::string DevHelper::PINS_DEVICE = "v2r_pins";
    const int DevHelper::INV_FILE_HANDLE = -1;
    const std::string DevHelper::DEV_PREFIX = "/dev/";
    const std::string DevHelper::ADC_DEVICE = "v2r_adc";
    const std::string DevHelper::GPIO_DEVICE = "v2r_gpio";
    
    DevHelper::DevHelper(const std::string& devName)
    : FileHelper(DEV_PREFIX + devName)
    {
    }
    
    DevHelper::~DevHelper()
    {
    }
    
    void DevHelper::sendCommand(const std::string& cmd)
    {
        if (!isOpened())
        {
            PC_EXCEPTION(InternalErrorException,
                    "Device doesn't open.");
        }
        
        std::clog << "Write command to device: " << cmd << std::endl;
        writeData(cmd.c_str(), cmd.size());
    }
    
    void DevHelper::doCommand(const std::string& devName, 
            const std::string& command)
    {
        DevHelper devHelper(devName);
        devHelper.sendCommand(command);
    }
}
