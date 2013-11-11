#include "pincontroller/adc_reader.hpp"
#include "details/dev_helper.hpp"
#include "pincontroller/exceptions.hpp"

namespace pc
{
    const float ADCReader::REFERENCE_VOLTAGE = 3.6;
    const uint16_t ADCReader::MAX_RAW_VALUE = (1 << 10) - 1;
    
    ADCReader::ADCReader()
    {
        for (size_t i = 0; i < ADC_COUNT; i++)
        {
            maxMeasurementVoltage[i] = REFERENCE_VOLTAGE;
            minMeasurementVoltage[i] = 0.0f;
        }
    }
    
    ADCReader::~ADCReader()
    {
        
    }
    
    void ADCReader::read(ADCValue& adValue)
    {
        uint16_t buffer[ADC_COUNT] = { 0 };
        
        memset(&adValue, 0, sizeof(adValue));
        
        DevHelper helper(DevHelper::ADC_DEVICE);
        if (helper.readData(buffer, sizeof(buffer)) != sizeof(buffer))
        {
            PC_EXCEPTION(DeviceException, 
                    "Unable to read data from ADC device.");
        }
        
        for (int i = 0; i < ADC_COUNT; i++)
        {
            adValue.adcValuesRaw[i] = buffer[i];
            adValue.adcVoltages[i] = adValue.adcValuesRaw[i] * 
                    REFERENCE_VOLTAGE / (MAX_RAW_VALUE + 1);
        }
        
        helper.close();
    }
    
    float ADCReader::getMaxMeasurementVoltage(size_t channel) const
    {
        if (channel >= ADC_COUNT)
        {
            PC_EXCEPTION(OutOfRangeException, "Invalid ADC channel index.");
        }

        return maxMeasurementVoltage[channel];
    }
    
    void ADCReader::setMaxMeasurementVoltage(float v)
    {
        for (size_t i = 0; i < ADC_COUNT; i++)
        {
            setMaxMeasurementVoltage(v, i);
        }
    }
    
    void ADCReader::setMaxMeasurementVoltage(float v, size_t channel)
    {
        if (channel >= ADC_COUNT)
        {
            PC_EXCEPTION(OutOfRangeException, "Invalid ADC channel index.");
        }
        
        maxMeasurementVoltage[channel] = v;
    }

    float ADCReader::getMinMeasurementVoltage(size_t channel) const
    {
        if (channel >= ADC_COUNT)
        {
            PC_EXCEPTION(OutOfRangeException, "Invalid ADC channel index.");
        }

        return minMeasurementVoltage[channel];
    }
    
    void ADCReader::setMinMeasurementVoltage(float v)
    {
        for (size_t i = 0; i < ADC_COUNT; i++)
        {
            setMinMeasurementVoltage(v, i);
        }
    }
    
    void ADCReader::setMinMeasurementVoltage(float v, size_t channel)
    {
        if (channel >= ADC_COUNT)
        {
            PC_EXCEPTION(OutOfRangeException, "Invalid ADC channel index.");
        }
        
        minMeasurementVoltage[channel] = v;
    }

    void ADCReader::readScaled(ADCValue& adValue)
    {
        if (maxMeasurementVoltage <= minMeasurementVoltage)
        {
            PC_EXCEPTION(IncorrectParamException,
                    "Maximal measurement voltage should be greater "
                    "than minimal measurement voltage.");
        }
        
        read(adValue);
        
        for (int i = 0; i < ADC_COUNT; i++)
        {
            adValue.adcVoltages[i] = adValue.adcValuesRaw[i] * 
                    (maxMeasurementVoltage[i] - minMeasurementVoltage[i]) / 
                    (MAX_RAW_VALUE + 1) + minMeasurementVoltage[i];
        }
    }
}
