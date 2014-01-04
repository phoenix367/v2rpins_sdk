/* 
 * File:   Exceptions.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 11, 2013, 1:36 AM
 */

#ifndef COMM_SERVER_EXCEPTIONS_HPP
#define	COMM_SERVER_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>
#include <sstream>

#define COMM_EXCEPTION(ex, msg) (throw ex(msg, __LINE__, __FILE__))

class Exception : public std::exception
{
public:
    Exception(const std::string& msg, int line, const char* file) throw()
    {
        std::ostringstream stream;
        
        stream << "Throw exception in file " << file << " at line " <<
                line << ": " << msg;
        message = stream.str();
    }
    
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
        
private:
    std::string message;
};

class FileException : public Exception
{
public:
    FileException(const std::string& msg, int line, const char* file) throw()
    : Exception(msg, line, file)
    {
        
    }
};

class NullPointerException : public Exception
{
public:
    NullPointerException(const std::string& msg, int line, const char* file) throw()
    : Exception(msg, line, file)
    {
        
    }
};

class ConfigurationException : public Exception
{
public:
    ConfigurationException(const std::string& msg, int line, const char* file) throw()
    : Exception(msg, line, file)
    {
        
    }
};

class OutOfRangeException : public Exception
{
public:
    OutOfRangeException(const std::string& msg, int line, const char* file) throw()
    : Exception(msg, line, file)
    {
        
    }
};

class LaunchException : public Exception
{
public:
    LaunchException(const std::string& msg, int line, const char* file) throw()
    : Exception(msg, line, file)
    {
        
    }
};

class InternalError : public Exception
{
public:
    InternalError(const std::string& msg, int line, const char* file) throw()
    : Exception(msg, line, file)
    {
        
    }
};

#endif	/* COMM_SERVER_EXCEPTIONS_HPP */
