/* 
 * File:   reference.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 22:19
 */

#ifndef REFERENCE_HPP
#define	REFERENCE_HPP

#include <memory>

#include "exceptions.hpp"

namespace pc
{
    template<typename T> class Reference
    {
    protected:
        typedef std::shared_ptr<T> ImplPtr;
            
        ImplPtr check()
        {
            if (!implPtr)
            {
                PC_EXCEPTION(ObjectExpiredException,
                        "Implementation object is expired.");
            }
            
            return implPtr;
        }

    protected:
        ImplPtr implPtr;
    };
}

#endif	/* REFERENCE_HPP */
