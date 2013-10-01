/* 
 * File:   exceptions.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 19:16
 */

#ifndef EXCEPTIONS_HPP
#define	EXCEPTIONS_HPP

#include <stdexcept>
#include <cstring>
#include <string>
#include <sstream>

#define PC_EXCEPTION(ex, arg) throw(ex(arg, __LINE__, __FILE__))

namespace pc
{
    class Exception : public std::exception
    {
    public:
        Exception(const std::string& msg, int line, 
                const std::string& file) throw()
                : exception()
                , exLine(line)
                , exFile(file)
        {
            std::ostringstream stream;
            stream << "Exception occured at " << exFile << ", line " <<
                    exLine << ": " << msg;
            message = stream.str();
        }
        
        virtual ~Exception() throw()
        {
        
        }
        
        virtual const char* what() const throw()
        {
            return message.c_str();
        }
        
    protected:
        std::string message;
        int exLine;
        std::string exFile;
    };
    
    class DeviceException : public Exception
    {
    public:
        DeviceException(const std::string& msg, int line, 
                const std::string& file) throw()
                : Exception(msg, line, file)
        {
        }

        DeviceException(int errnoValue, int line, 
                const std::string& file) throw()
                : Exception(strerror(errnoValue), line, file)
        {
        }
        
        virtual ~DeviceException() throw()
        {
        
        }
    };

    class ObjectExpiredException : public Exception
    {
    public:
        ObjectExpiredException(const std::string& msg, int line, 
                const std::string& file) throw()
                : Exception(msg, line, file)
        {
        }
        
        virtual ~ObjectExpiredException() throw()
        {
        
        }
    };

    class InternalErrorException : public Exception
    {
    public:
        InternalErrorException(const std::string& msg, int line, 
                const std::string& file) throw()
                : Exception(msg, line, file)
        {
        }
        
        virtual ~InternalErrorException() throw()
        {
        
        }
    };

    class OutOfRangeException : public Exception
    {
    public:
        OutOfRangeException(const std::string& msg, int line, 
                const std::string& file) throw()
                : Exception(msg, line, file)
        {
        }
        
        virtual ~OutOfRangeException() throw()
        {
        
        }
    };

    class PinLockedException : public Exception
    {
    public:
        PinLockedException(const std::string& msg, int line, 
                const std::string& file) throw()
                : Exception(msg, line, file)
        {
        }
        
        virtual ~PinLockedException() throw()
        {
        
        }
    };

    class IncorrectParamException : public Exception
    {
    public:
        IncorrectParamException(const std::string& msg, int line, 
                const std::string& file) throw()
                : Exception(msg, line, file)
        {
        }
        
        virtual ~IncorrectParamException() throw()
        {
        
        }
    };
}

#endif	/* EXCEPTIONS_HPP */
