/* 
 * File:   adc_reader.hpp
 * Author: Ivan Gubochkin
 *
 * Created on September 30, 2013, 11:10 PM
 */

/**
 * @file adc_reader.hpp
 * @brief Заголовочный файл класса ADCReader
 */
#ifndef ADC_READER_HPP
#define	ADC_READER_HPP

#include <cstdint>
#include <cstddef>

namespace pc
{
    /**
     * @class ADCReader
     * @brief Класс предоставляет функциональность для считывания
     * значений с установленного на плате virt2real аналого-цифрового
     * преобразователя (далее АЦП).
     * @details Установленный на плате АЦП имеет 6 каналов. Он имеет 10
     * разрядов и дапазон изменения входного напряжения от 0 до 3.6 вольта
     * (значения опрного напряжения Vref).
     * 
     * Класс ADCReader позволяет считывать данные одновременно со всех 6 
     * каналов АЦП как непосредственно в двоичном коде, так и 
     * преобразованные к уровню входного напряжения.
     */
    class ADCReader
    {
    public:
        /**
         * @brief Число доступных каналов АЦП
         */
        static const size_t ADC_COUNT = 6;
        
        /**
         * @brief Величина опорного напряжения Vref=3,6 В
         * @details Vref определяет максимальное измеряемое напряжение
         * на входе АЦП
         */
        static const float REFERENCE_VOLTAGE;
        
        /**
         * @brief Максимальное выходное значение АЦП. Равно 2^10-1 = 1023
         */
        static const uint16_t MAX_RAW_VALUE;
        
        /**
         * @struct ADCValue
         * @brief Структура для хранения результата считаывания
         * такущих значений напряжения на входах АЦП
         */
        struct ADCValue
        {
            /**
             * @brief Значения в двоичном коде от 0 до 1023
             */
            uint16_t adcValuesRaw[ADC_COUNT];
            
            /**
             * @brief Значения, приведенные к входному напряжению
             * в вольтах
             */
            float adcVoltages[ADC_COUNT];
        };
        
    public:
        /**
         * @brief Конструктор класса
         */
        ADCReader();
        
        /**
         * @brief Деструктор класса. Уничтожает объект.
         */
        virtual ~ADCReader();
        
        /**
         * @brief Функция считывает текущие значения напряжения
         * на входах АЦП
         * @param adValue [out] структура типа ADCValue, содержащая
         * прочитанные данные.
         * @throw DeviceException - выбрасывается, если операция
         * считывания не может быть произведена
         * @details Функция заполняет стурктуру ADCValue. При этом
         * перевод значений из двоичного представления во входное
         * напряжение производится в предположении, что ко входам
         * не подключены дополнительно какие-либо резистивные
         * делители или преобразователи напряжения. Таким образом
         * оцифрованное значение будет находиться в пределах от 0 до
         * 3,6 В и соответствовует входному напряжению как 1:1
         */
        void read(ADCValue& adValue);
        
        /**
         * @brief Функция считывает текущие значения напряжения
         * на входах АЦП и масштабирует полученный результат
         * @param adValue [out] структура типа ADCValue, содержащая
         * прочитанные данные.
         * @throw DeviceException - выбрасывается, если операция
         * считывания не может быть произведена
         * @throw DeviceException - выбрасывается, если установленное
         * максимальное значение входного напряжения меньше, либо равно
         * минимальному значению
         * @details Функция заполняет стурктуру ADCValue. При этом
         * перевод значений из двоичного представления во входное
         * напряжение производится по формуле 
         * Vin=ADC_value * (Vmax - Vmin) / 1023, где ADC_value - двоичное
         * значение, прочитанное с АЦП, Vmax - значение максимального
         * измеряемого напряжения на соответствующем канале АЦП, 
         * установленного ранее с помощью
         * функции setMaxMeasurementVoltage, а Vmin - значение минимального
         * измеряемого напряжения на соответствующем канале АЦП,
         * установленного ранее с помощью
         * функции setMinMeasurementVoltage. По умолчанию Vmax=Vref, а
         * Vmin=0 В. В этому случае результат, возвращаемый функцией 
         * идентичен тому, который вернет read.
         * 
         * Функция readScaled может использоваться в случае, когда
         * на входах АЦП установлены резистичные делители или 
         * преобразователи напряжения для изменения диапазона измеряемых
         * входных напряжений.
         */
        void readScaled(ADCValue& adValue);
        
        /**
         * @brief Возвращает установленное значение максимального
         * измеряемого напряжения на входе АЦП на заданном
         * канале
         * @param channel номер канала АЦП в диапазоне от 0 до 5
         * @return максимальное измеряемое напряжение в вольтах
         * @throw OutOfRangeException в случае, если номер
         * канала АЦП указан неправильно
         */
        float getMaxMeasurementVoltage(size_t channel) const;
        
        /**
         * @brief Устанавливает значение максимального
         * измеряемого напряжения на входе АЦП одновременно
         * для всех каналов
         * @param v максимальное измеряемое напряжение в вольтах
         */
        void setMaxMeasurementVoltage(float v);
        
        /**
         * @brief Устанавливает значение максимального
         * измеряемого напряжения на входе АЦП только для
         * заданного канала
         * @param v максимальное измеряемое напряжение в вольтах
         * @param channel номер канала АЦП в диапазоне от 0 до 5
         * @throw OutOfRangeException в случае, если номер
         * канала АЦП указан неправильно
         */
        void setMaxMeasurementVoltage(float v, size_t channel);
        
        /**
         * @brief Возвращает установленное значение минимального
         * измеряемого напряжения на входе АЦП
         * @param channel номер канала АЦП в диапазоне от 0 до 5
         * @return минимальное измеряемое напряжение в вольтах
         * @throw OutOfRangeException в случае, если номер
         * канала АЦП указан неправильно
         */
        float getMinMeasurementVoltage(size_t channel) const;
        
        /**
         * @brief Устанавливает значение минимального
         * измеряемого напряжения на входе АЦП одновременно
         * для всех каналов
         * @param v минимальное измеряемое напряжение в вольтах
         */
        void setMinMeasurementVoltage(float v);
        
        /**
         * @brief Устанавливает значение минимального
         * измеряемого напряжения на входе АЦП только для
         * заданного канала
         * @param v минимальное измеряемое напряжение в вольтах
         * @param channel номер канала АЦП в диапазоне от 0 до 5
         * @throw OutOfRangeException в случае, если номер
         * канала АЦП указан неправильно
         */
        void setMinMeasurementVoltage(float v, size_t channel);
        
    private:
        float maxMeasurementVoltage[ADC_COUNT];
        float minMeasurementVoltage[ADC_COUNT];
    };
}

#endif	/* ADC_READER_HPP */
