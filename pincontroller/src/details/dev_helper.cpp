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
    
    DevHelper::DevHelper(const std::string& devName)
    : devFileHandle(INV_FILE_HANDLE)
    {
        open(devName);
    }
    
    DevHelper::~DevHelper()
    {
        close();
    }
    
    void DevHelper::open(const std::string& devName)
    {
        close();
        
        std::string name = DEV_PREFIX + devName;
        int handle = ::open(name.c_str(), O_RDWR | O_NDELAY);
        if (handle == INV_FILE_HANDLE)
        {
            PC_EXCEPTION(DeviceException, errno);
        }
        
        devFileHandle = handle;
    }
    
    void DevHelper::close()
    {
        if (isOpened())
        {
            ::close(devFileHandle);
        }
    }
    
    bool DevHelper::isOpened()
    {
        return devFileHandle != INV_FILE_HANDLE;
    }
    
    void DevHelper::sendCommand(const std::string& cmd)
    {
        if (!isOpened())
        {
            PC_EXCEPTION(InternalErrorException,
                    "Device doesn't open.");
        }
        
        std::clog << "Write command to device: " << cmd << std::endl;

        if (::write(devFileHandle, cmd.c_str(), cmd.size()) == -1)
        {
            PC_EXCEPTION(DeviceException, errno);
        }
    }
    
    void DevHelper::doCommand(const std::string& devName, 
            const std::string& command)
    {
        DevHelper devHelper(devName);
        devHelper.sendCommand(command);
    }
    
    int DevHelper::readData(void* data, size_t readSize)
    {
        int r = ::read(devFileHandle, data, readSize);
        if (r == -1)
        {
            PC_EXCEPTION(DeviceException, errno);
        }
        
        return r;
    }
}
