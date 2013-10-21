/* 
 * File:   global.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 21:51
 */

#ifndef PC_GLOBAL_HPP
#define	PC_GLOBAL_HPP

namespace pc
{
    /**
     * @enum PWM_CHANNEL
     * @brief Перечисление задает идентификаторы каналов
     * аппаратного PWM.
     */
    enum class PWM_CHANNEL : int
    {
        PWM_0 = 0, /** Канал PWM 0 */
        PWM_1, /** Канал PWM 1 */
        PWM_2, /** Канал PWM 2 */
        PWM_3, /** Канал PWM 3 */
        PWM_MAX /** Определяет максимальное число каналов */
    };

    /**
     * @enum GPIO_PIN
     */
    enum class GPIO_PIN : int
    {
        gpio1  =  1,
        gpio2  =  2,
        gpio3  =  3,
        gpio4  =  4,
        gpio5  =  5,
        gpio6  =  6,
        gpio7  =  7,
        gpio8  =  8,
        gpio9  =  9,
        gpio10 = 10,
        gpio11 = 11,
        gpio12 = 12,
        gpio13 = 13,
        gpio14 = 14,
        gpio15 = 15,
        gpio22 = 22,
        gpio23 = 23,
        gpio24 = 24,
        gpio25 = 25,
        gpio26 = 26,
        gpio27 = 27,
        gpio28 = 28,
        gpio29 = 29,
        gpio30 = 30,
        gpio31 = 31,
        gpio32 = 32,
        gpio33 = 33,
        gpio34 = 34,
        gpio35 = 35,
        gpio36 = 36,
        gpio37 = 37,
        gpio44 = 44,
        gpio45 = 45,
        gpio46 = 46,
        gpio47 = 47,
        gpio48 = 48,
        gpio49 = 49,
        gpio50 = 50,
        gpio79 = 79,
        gpio80 = 80,
        gpio81 = 81,
        gpio82 = 82,
        gpio83 = 83,
        gpio84 = 84,
        gpio85 = 85,
        gpio86 = 86,
        gpio87 = 87,
        gpio88 = 88,
        gpio89 = 89,
        gpio90 = 90,
        gpio91 = 91,
        gpio92 = 92
    };
    
    /**
     * @enum GPIO_DIRECTION
     */
    enum class GPIO_DIRECTION
    {
        input,
        output
    };
    
    enum class GPIO_LOGIC_LEVEL : int
    {
        low  = 0,
        high = 1
    };
}
#endif	/* PC_GLOBAL_HPP */
