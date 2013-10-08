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
        gpio1,
        gpio2,
        gpio3,
        gpio4,
        gpio5,
        gpio6,
        gpio7,
        gpio8,
        gpio9,
        gpio10,
        gpio11,
        gpio12,
        gpio13,
        gpio14,
        gpio15,
        gpio22,
        gpio23,
        gpio24,
        gpio25,
        gpio26,
        gpio27,
        gpio28,
        gpio29,
        gpio30,
        gpio31,
        gpio32,
        gpio33,
        gpio34,
        gpio35,
        gpio36,
        gpio37,
        gpio44,
        gpio45,
        gpio46,
        gpio47,
        gpio48,
        gpio49,
        gpio50,
        gpio79,
        gpio80,
        gpio81,
        gpio82,
        gpio83,
        gpio84,
        gpio85,
        gpio86,
        gpio87,
        gpio88,
        gpio89,
        gpio90,
        gpio91,
        gpio92
    };
}
#endif	/* PC_GLOBAL_HPP */
