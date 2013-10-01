/* 
 * File:   adc_reader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on September 30, 2013, 11:10 PM
 */

#ifndef ADC_READER_HPP
#define	ADC_READER_HPP

#include <cstdint>

namespace pc
{
    class ADCReader
    {
    public:
        /**
         * 
         */
        static const int ADC_COUNT = 6;
        
        /**
         * 
         */
        static const float REFERENCE_VOLTAGE;
        
        /**
         *
         */
        static const uint16_t MAX_RAW_VALUE;
        
        /**
         * 
         */
        struct ADCValue
        {
            /**
             * 
             */
            uint16_t adcValuesRaw[ADC_COUNT];
            
            /**
             * 
             */
            float adcVoltages[ADC_COUNT];
        };
        
    public:
        /**
         * 
         */
        ADCReader();
        
        /**
         * 
         */
        virtual ~ADCReader();
        
        /**
         * 
         * @param adValue
         */
        void read(ADCValue& adValue);
    };
}

#endif	/* ADC_READER_HPP */
