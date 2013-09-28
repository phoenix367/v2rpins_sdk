/* 
 * File:   pwm.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 22:35
 */

/**
 * @file pwm.hpp
 * @brief Заголовочный файл класса PWM
 */
#ifndef PWM_HPP
#define	PWM_HPP

#include "global.hpp"
#include "reference.hpp"

#include <cstdint>

namespace pc
{
    class PWMImpl;
    
    /**
     * @class PWM
     * @brief Класс предназначен для управления аппаратным PWM
     * (Pulse-width modulator)платы virt2real версии 1.1. 
     * Всего данная плата позволяет
     * задействовать четыре канала PWM: pwm0, pwm1, pwm2
     * и pwm3.
     */
    class PWM : public Reference<PWMImpl>
    {
    public:
        /**
         * @brief Конструктор по умолчанию. Инициализирует
         * пустой экземпляр класса PWM.
         */
        PWM();
        
        /**
         * @brief Конструктор создает объект класса PWM, привязанный
         * к заданному каналу аппаратного PWM.
         * @param output идентификатор канала PWM
         * 
         * @throw InternalErrorException в случае ошибки во внутренней
         * логике
         * 
         * @throw PinLockedException в случае, если выбранный канал
         * PWM уже используется.
         */
        PWM(PWM_CHANNEL output);
        
        /**
         * @brief Деструктор класса, вызывается во время разрушения
         * объекта.
         */
        virtual ~PWM();
        
        /**
         * @brief Устанавливает параметры генерации импульсов
         * 
         * @param duty длительность импульса в микросекундах
         * @param period период импульса в микросекундах
         * 
         * @throw ObjectExpiredException в случае, если реализация
         * объекта была разрушена
         * @throw IncorrectParamException в случае, если устанавливаемая
         * длительность периода меньше длительности импульса
         */
        void setPulseParams(uint32_t duty, uint32_t period);
        
        /**
         * @brief Запускает генерацию импульсов с заданными параметрами
         * на выбраном канале PWM
         */
        void init();
        
        /**
         * @brief Останавливает выдачу импульсов на соответствующем 
         * канале PWM
         */
        void stop();
        
        /**
         * @brief Функция возвращает текущее значение длительности
         * импульса PWM в микросекундах
         * @return длительность импульса PWM
         * @throw ObjectExpiredException
         */
        uint32_t getDuty() const;

        /**
         * @brief Функция возвращает текущее значение длительности
         * периода PWM в микросекундах
         * @return длительность периода PWM
         * @throw ObjectExpiredException
         */
        uint32_t getPeriod() const;

        /**
         * @brief Функция возвращает идентификатор используемого 
         * канала PWM
         * @return идентификатор канала PWM
         * @throw ObjectExpiredException
         */
        PWM_CHANNEL getPWMPin() const;
        
        /**
         * @brief Разрушает внутреннюю ссылку на объект управления PWM.
         * В случае, если нет других ссылок на тот же самый объект,
         * то он уничтожается, а соответствующий канал PWM 
         * освобождается.
         * 
         * @details Простой пример для демонстрации принципа работы.
         * 
         * @code
         * pc::PWM pwm1(pc::PWM_CHANNEL::PWM_0);
         * foo(pwm1);
         * pwm1.dispose(); // Здесь канал PWM_0 освобождается
         * ...................
         * void foo(pc::PWM& p)
         * {
         *      pc::PWM pwm2 = p;
         *      // что-то делаем с pwm2
         *      .......................
         *      pwm2.dispose();
         *      // По завершении функции объект pwm2 разрушится,
         *      // но возможность управления каналом PWM_0 из объекта
         *      // pwm1 сохранится.
         * }
         */
        void dispose();
                
        /**
         * @brief Освобождает занятый канал PWM. 
         * @details Для повторного доступа
         * к освобожденному каналу нужно вновь создать объект класса 
         * PWM с заданием соответствующего номера канала. 
         * Ранее созданные объекты, которые были привязаны к 
         * освобождаемому каналу больше не смогут его использовать.
         */
        void releasePwmChannel();
    };
}

#endif	/* PWM_HPP */
