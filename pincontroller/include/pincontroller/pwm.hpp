/* 
 * File:   pwm.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 22 Сентябрь 2013 г., 22:35
 */

/*
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
    
    /* 
     * Класс предназначен для управления аппаратным PWM
     * (Pulse-width modulator)платы virt2real версии 1.1. 
     * Всего данная плата позволяет
     * задействовать четыре канала PWM: pwm0, pwm1, pwm2
     * и pwm3.
     */
    class PWM : public Reference<PWMImpl>
    {
    public:
        /*
         * Конструктор по умолчанию. Инициализирует
         * пустой экземпляр класса PWM.
         */
        PWM();
        
        /*
         * Конструктор создает объект класса PWM, привязанный
         * к заданному каналу аппаратного PWM.
         * @param output идентификатор канала PWM
         * @throw InternalErrorException в случае ошибки во внутренней
         * логике
         */
        PWM(PWM_CHANNEL output);
        
        /*
         * Деструктор класса, вызывается во время разрушения
         * объекта.
         */
        virtual ~PWM();
        
        /*
         * Устанавливает параметры генерации импульсов
         * 
         * @param duty длительность импульса в микросекундах
         * @param period период импульса в микросекундах
         * 
         * @throw 
         * ObjectExpiredException в случае, если реализация
         * объекта была разрушена
         *        
         * IncorrectParamException в случае, если устанавливаемая
         * длительность периода меньше длительности импульса
         */
        void setPulseParams(uint32_t duty, uint32_t period);
        
        /*
         */
        void init();
        
        /*
         * Останваливает выбачу импульсов на соответствующем 
         * канале PWM
         */
        void stop();
        
        /*
         * Функция возвращает текущее значение длительности
         * импульса PWM в микросекундах
         * @return длительность импульса PWM
         * @throw ObjectExpiredException
         */
        uint32_t getDuty();

        /*
         * Функция возвращает текущее значение длительности
         * периода PWM в микросекундах
         * @return длительность периода PWM
         * @throw ObjectExpiredException
         */
        uint32_t getPeriod();

        /*
         * Функция возвращает идентификатор используемого 
         * канала PWM
         * @return идентификатор канала PWM
         * @throw ObjectExpiredException
         */
        PWM_CHANNEL getPWMPin();
    };
}

#endif	/* PWM_HPP */
