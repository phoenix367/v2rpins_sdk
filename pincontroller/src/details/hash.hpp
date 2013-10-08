/* 
 * File:   hash.hpp
 * Author: Ivan Gubochkin
 *
 * Created on October 8, 2013, 10:23 PM
 */

#ifndef HASH_HPP
#define	HASH_HPP

#include <type_traits>
#include <functional>

#include "pincontroller/global.hpp"
#include "internal_enums.hpp"

namespace std
{
    template<>
    struct hash< pc::PWM_CHANNEL >
    {
        typedef pc::PWM_CHANNEL argument_type;
        typedef std::underlying_type< argument_type >::type underlying_type;
        typedef std::hash< underlying_type >::result_type result_type;
        result_type operator()( const argument_type& arg ) const
        {
            std::hash< underlying_type > hasher;
            return hasher( static_cast< underlying_type >( arg ) );
        }
    };

    template<>
    struct hash< pc::Contact >
    {
        typedef pc::Contact argument_type;
        typedef std::underlying_type< argument_type >::type underlying_type;
        typedef std::hash< underlying_type >::result_type result_type;
        result_type operator()( const argument_type& arg ) const
        {
            std::hash< underlying_type > hasher;
            return hasher( static_cast< underlying_type >( arg ) );
        }
    };

    template<>
    struct hash< pc::GPIO_PIN >
    {
        typedef pc::GPIO_PIN argument_type;
        typedef std::underlying_type< argument_type >::type underlying_type;
        typedef std::hash< underlying_type >::result_type result_type;
        result_type operator()( const argument_type& arg ) const
        {
            std::hash< underlying_type > hasher;
            return hasher( static_cast< underlying_type >( arg ) );
        }
    };
}

#endif	/* HASH_HPP */
