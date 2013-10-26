#include "file_helper.hpp"
#include "pincontroller/exceptions.hpp"

#include <unistd.h>
#include <errno.h>

namespace pc
{
    const int FileHelper::INV_FILE_HANDLE = -1;

    FileHelper::FileHelper(const std::string& fileName, int attr)
    : devFileHandle(INV_FILE_HANDLE)
    {
        open(fileName, attr);
    }
    
    FileHelper::~FileHelper()
    {
        close();
    }
    
    void FileHelper::open(const std::string& fileName, int attr)
    {
        close();
        
        int handle = ::open(fileName.c_str(), attr);
        if (handle == INV_FILE_HANDLE)
        {
            PC_EXCEPTION(DeviceException, errno);
        }
        
        devFileHandle = handle;
    }
    
    void FileHelper::close()
    {
        if (isOpened())
        {
            ::close(devFileHandle);
        }
    }
    
    bool FileHelper::isOpened()
    {
        return devFileHandle != INV_FILE_HANDLE;
    }

    int FileHelper::readData(void* data, size_t readSize)
    {
        int r = ::read(devFileHandle, data, readSize);
        if (r == -1)
        {
            PC_EXCEPTION(DeviceException, errno);
        }
        
        return r;
    }

    int FileHelper::writeData(const void* data, size_t writeSize)
    {
        int r = ::write(devFileHandle, data, writeSize);
        if (r == -1)
        {
            PC_EXCEPTION(DeviceException, errno);
        }
        
        return r;
    }
}
