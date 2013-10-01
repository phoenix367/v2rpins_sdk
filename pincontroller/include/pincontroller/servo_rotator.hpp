/* 
 * File:   servo_rotator.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 28 Сентябрь 2013 г., 17:40
 */

/**
 * @file servo_rotator.hpp
 * @brief Заголовочный файл класса управления 
 * сервоприводом
 */

#ifndef SERVO_ROTATOR_HPP
#define	SERVO_ROTATOR_HPP

#include "pwm.hpp"

namespace pc
{
    /**
     * @class ServoRotator
     * @brief Класс %ServoRotator предназначен для
     * управления сервоприводом.
     * @details Управление сервоприводом осуществляется
     * через аппаратный PWM, к одному и каналов которого
     * подключается привод. Класс предоставляет поворачивать
     * ось сервопривода на заданный угол, а также задавать
     * параметры следования импульсов управления. Для более
     * подробного описания принципов управления сервоприводом
     * с помощью платы virt2real см. проектную документацию
     * по ссылке http://wiki.virt2real.ru/wiki/PPM
     */
    class ServoRotator
    {
    public:
        /**
         * @brief Период управляющих импульсов по умолчанию.
         * Текущее значение 20000 микросекунд.
         */
        static const uint32_t DEFAULT_IMP_PERIOD;
        
        /**
         * @brief Минимальная управляющих импульсов по умолчанию.
         * Текущее значение 700 микросекунд.
         */
        static const uint32_t DEFAULT_MIN_DUTY;
        
        /**
         * @brief Максимальная управляющих импульсов по умолчанию.
         * Текущее значение 2200 микросекунд.
         */
        static const uint32_t DEFAULT_MAX_DUTY;
        
        /**
         * @brief Максимальный угол поворота оси сервопривода в градусах.
         * Текущее значение 180 градусов.
         */
        static const uint16_t DEFAULT_MAX_ANGLE;
        
    public:
        /**
         * @brief Конструктор создает объект класса %ServoRotator, 
         * привязанный к уже созданному каналу %PWM
         * @param extPwm объект управления каналом %PWM
         * @details Данный конструктор может быть использован
         * в случае, если управление сервоприводом будет идти
         * на уже созданном канала %PWM. Следует учитывать, что
         * при вызове методов класса %ServoRotator зачения настроек
         * канала %PWM могут меняться. После уничтожения экзепляра
         * класса %ServoRotator привязанный к нему объект управления 
         * каналом %PWM сохраняется. Параметры
         * генерации управлющих импульсов устанавливаются равными
         * значениям по умолчанию.
         */
        ServoRotator(PWM& extPwm);
        
        /**
         * @brief Конструктор создает объект класса %ServoRotator, 
         * привязанный к вновь создаваемому каналу %PWM
         * @param pwmChannel канал аппаратного %PWM
         * @throw PinLockedException в случае, если выбранный канал
         * %PWM уже используется
         * @details Конструктор создает объект класса %ServoRotator, 
         * привязанный к вновь создаваемому каналу %PWM. После 
         * уничтожения экзепляра класса %ServoRotator привязанный к 
         * нему объект управления каналом %PWM также уничтожается,
         * а связанные с ним ресурсы освобождаются. Параметры
         * генерации управлющих импульсов устанавливаются равными
         * значениям по умолчанию.
         */
        ServoRotator(PWM_CHANNEL pwmChannel);
        
        /**
         * @brief Деструктор уничетожает объект класса %ServoRotator
         */
        virtual ~ServoRotator();
        
        /**
         * @brief Функция возвращает период управляющих импульсов
         * @return период управляющих импульсов в микросекундах
         * @throw ObjectExpiredException в случае, если
         * объект управления %PWM устарел или уничтожен.
         */
        uint32_t getImpulsePeriod() const;
        
        /**
         * @brief Функция устанавливает новое значение периода
         * управляющих импульсов
         * @param newPeriod период управляющих импульсов в микросекундах
         * @throw ObjectExpiredException в случае, если
         * объект управления %PWM устарел или уничтожен.
         */
        void setImpulsePeriod(uint32_t newPeriod);
        
        /**
         * @brief Функция возвращает минимальную длительность управляющих
         * импульсов
         * @return Минимальная длительность управляющих импульсов в 
         * микросекундах
         * @details Под минимальной длительностью управляющих импульсов
         * понимается такая длительность при которой ось сервопривода
         * поворачивается на 0 градусов.
         */
        uint32_t getMinDuty() const;
        
        /**
         * @brief Функция задает минимальную длительность управляющих
         * импульсов
         * @param newMinDuty минимальная длительность управляющих импульсов в 
         * микросекундах
         * @details Под минимальной длительностью управляющих импульсов
         * понимается такая длительность при которой ось сервопривода
         * поворачивается на 0 градусов. Ее значение определяется
         * характеристиками используемого серовпривода.
         */
        void setMinDuty(uint32_t newMinDuty);
        
        /**
         * @brief Функция возвращает максимальную длительность 
         * управляющих импульсов
         * @return максимальная длительность управляющих импульсов в
         * микросукендах
         * @details Под максимальной длительностью управляющих импульсов
         * понимается такая длительность при которой ось сервопривода
         * поворачивается на максимально возможный угол (обычно 180 градусов,
         * но существуют сервоприводы и с другим максимальным углом поворота).
         */
        uint32_t getMaxDuty() const;
        
        /**
         * @brief Функция устанавливает максимальную длительность 
         * управляющих импульсов
         * @param newMaxDuty максимальная длительность управляющих импульсов в
         * микросекундах
         * @details Под максимальной длительностью управляющих импульсов
         * понимается такая длительность при которой ось сервопривода
         * поворачивается на максимально возможный угол (обычно 180 градусов,
         * но существуют сервоприводы и с другим максимальным углом поворота).
         * Ее значение определяется характеристиками используемого 
         * серовпривода.
         */
        void setMaxDuty(uint32_t newMaxDuty);
        
        /**
         * @brief Поворачивает ось сервопривода на заданный угол
         * @param angle угол поворота в градусах
         * @throw ObjectExpiredException в случае, если
         * объект управления %PWM устарел или уничтожен.
         * @throw OutOfRangeException в случае, если заданный угол
         * поворота превышает максимально допустимый
         * @throw IncorrectParamException при неправильно установленных
         * параметрах управляющих импульсов.
         * @details Для того, чтобы угол поворота, заданный программно
         * и реальный угол поворота сервопривода совпадали необходимо
         * устанавливать корректные параметры управляющих импульсов.
         * Данные параметры определяются моделью используемого
         * сервопривода.
         */
        void rotate(uint16_t angle);
        
        /**
         * @brief Функция возвращает максимально возможный угол поворота
         * оси сервопривода
         * @return максимально возможный угол поворота
         * оси сервопривода в градусах
         * @details Максимально возможный угол поворота
         * оси сервопривода определяется его техническими характеристиками
         * и составляет обычно 180 градусов.
         */
        uint16_t getMaxAngle();
        
        /**
         * @brief Функция устанавливает максимально возможный угол поворота
         * оси сервопривода
         * @param newMaxAngle максимально возможный угол поворота
         * оси сервопривода в градусах
         * @details Максимально возможный угол поворота
         * оси сервопривода определяется его техническими характеристиками
         * и составляет обычно 180 градусов.
         */
        void setMaxAngle(uint16_t newMaxAngle);
        
    private:
        void init();
        
    private:
        PWM pwm;
        uint32_t impPeriod;
        uint32_t minDuty;
        uint32_t maxDuty;
        uint16_t maxAngle;
    };
}

#endif	/* SERVO_ROTATOR_HPP */