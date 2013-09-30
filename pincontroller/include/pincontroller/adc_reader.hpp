/* 
 * File:   adc_reader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on September 30, 2013, 11:10 PM
 */

#ifndef ADC_READER_HPP
#define	ADC_READER_HPP

#include <functional>
#include <memory>
#include <cstdint>

namespace pc
{
    class ADCReaderImpl;
    
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
        struct ADCValue
        {
            uint16_t adcValuesRaw[ADC_COUNT];
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
        static void readOnce(ADCValue& adValue);
        
    private:
        
    private:
        std::shared_ptr<ADCReaderImpl> impl;
    };
}

#endif	/* ADC_READER_HPP */
