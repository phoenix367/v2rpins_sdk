#include "pincontroller/adc_reader.hpp"
#include "details/dev_helper.hpp"
#include "pincontroller/exceptions.hpp"

namespace pc
{
    const float ADCReader::REFERENCE_VOLTAGE = 1.43;
    const uint16_t ADCReader::MAX_RAW_VALUE = 1 << 10;
    
    ADCReader::ADCReader()
    {
        
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
                    REFERENCE_VOLTAGE / MAX_RAW_VALUE;
        }
        
        helper.close();
    }
}
