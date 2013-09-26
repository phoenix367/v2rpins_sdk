/* 
 * File:   global.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 21:51
 */

#ifndef PC_GLOBAL_HPP
#define	PC_GLOBAL_HPP

#include <type_traits>
#include <functional>

namespace pc
{
    /*
     * @brief Перечисление задает идентификаторы каналов
     * аппаратного PWM.
     */
    enum class PWM_CHANNEL : int
    {
        PWM_0 = 0, /* Канал PWM 0 */
        PWM_1, /* Канал PWM 1 */
        PWM_2, /* Канал PWM 2 */
        PWM_3, /* Канал PWM 3 */
        PWM_MAX /* Определяет максимальное число каналов */
    };

}

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
}
#endif	/* PC_GLOBAL_HPP */
