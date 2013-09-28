/* 
 * File:   servo_rotator.hpp
 * Author: Ivan Gubochkin
 *
 * Created on 28 Сентябрь 2013 г., 17:40
 */

/**
 * @file servo_rotator.hpp
 * @brief
 */

#ifndef SERVO_ROTATOR_HPP
#define	SERVO_ROTATOR_HPP

#include "pwm.hpp"

namespace pc
{
    /**
     * @class ServoRotator
     * @brief
     */
    class ServoRotator
    {
    public:
        /**
         */
        static const uint32_t DEFAULT_IMP_PERIOD;
        
        /**
         */
        static const uint32_t DEFAULT_MIN_DUTY;
        
        /**
         */
        static const uint32_t DEFAULT_MAX_DUTY;
        
        /**
         */
        static const uint16_t DEFAULT_MAX_ANGLE;
        
    public:
        /**
         * 
         * @param extPwm
         */
        ServoRotator(PWM& extPwm);
        
        /**
         * 
         * @param pwmChannel
         */
        ServoRotator(PWM_CHANNEL pwmChannel);
        
        /**
         * 
         */
        virtual ~ServoRotator();
        
        /**
         * 
         * @return 
         */
        uint32_t getImpulsePeriod() const;
        
        /**
         * 
         * @param newPeriod
         */
        void setImpulsePeriod(uint32_t newPeriod);
        
        /**
         * 
         * @return 
         */
        uint32_t getMinDuty() const;
        
        /**
         * 
         * @param newMinDuty
         */
        void setMinDuty(uint32_t newMinDuty);
        
        /**
         * 
         * @return 
         */
        uint32_t getMaxDuty() const;
        
        /**
         * 
         * @param newMaxDuty
         */
        void setMaxDuty(uint32_t newMaxDuty);
        
        /**
         * 
         * @param degree
         */
        void rotate(uint16_t angle);
        
        /**
         * 
         * @return 
         */
        uint16_t getMaxAngle();
        
        /**
         * 
         * @param newMaxAngle
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
