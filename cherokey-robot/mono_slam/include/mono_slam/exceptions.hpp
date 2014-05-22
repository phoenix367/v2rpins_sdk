/* 
 * File:   exceptions.hpp
 * Author: Ivan Gubochkin
 *
 * Created on May 19, 2014, 12:46 AM
 */

#ifndef MSLAM_EXCEPTIONS_HPP
#define	MSLAM_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>
#include <sstream>

#define SLAM_EXCEPTION(ex, msg) (throw ex(msg, __LINE__, __FILE__))

namespace mslam
{
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
        
        virtual ~Exception() throw()
        {
            
        }

        virtual const char* what() const throw()
        {
            return message.c_str();
        }

    private:
        std::string message;
    };

    class IncorrectParamException : public Exception
    {
    public:
        IncorrectParamException(const std::string& msg, int line, 
                const char* file) throw()
        : Exception(msg, line, file)
        {

        }
    };
}

#endif	/* MSLAM_EXCEPTIONS_HPP */
