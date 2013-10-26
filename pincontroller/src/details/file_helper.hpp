/* 
 * File:   file_helper.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 26, 2013, 10:53 PM
 */

#ifndef FILE_HELPER_HPP
#define	FILE_HELPER_HPP

#include <string>
#include <fcntl.h>

namespace pc
{
    class FileHelper
    {
    public:
        static const int INV_FILE_HANDLE;
        
    public:
        FileHelper(const std::string& fileName, int attr = O_RDWR | O_NDELAY);
        virtual ~FileHelper();
        
        void open(const std::string& fileName, int attr);
        void close();
        bool isOpened();
        int readData(void* data, size_t readSize);
        int writeData(const void* data, size_t writeSize);
        
    protected:
        int devFileHandle;
    };
}

#endif	/* FILE_HELPER_HPP */
