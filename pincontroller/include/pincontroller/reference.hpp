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
    /**
     * @class Reference
     * @brief 
     */
    template<typename T> class Reference
    {
    protected:
        typedef std::shared_ptr<T> ImplPtr;
        typedef std::weak_ptr<T> WeakPtr;

        ImplPtr check()
        {
            if (implPtr.expired())
            {
                PC_EXCEPTION(ObjectExpiredException,
                        "Implementation object is expired.");
            }
            
            return implPtr.lock();
        }

        const ImplPtr check() const
        {
            if (implPtr.expired())
            {
                PC_EXCEPTION(ObjectExpiredException,
                        "Implementation object is expired.");
            }
            
            return implPtr.lock();
        }

    public:
        /**
         * @brief Проверяет, действителен ли еще объект
         * @return результат проверки
         */
        bool isExpired()
        {
            return implPtr.expired();
        }

    protected:
        WeakPtr implPtr;
    };
}

#endif	/* REFERENCE_HPP */
