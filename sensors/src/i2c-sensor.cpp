#include "sensors/i2c-sensor.hpp"
#include "file_helper.hpp"

namespace sn
{
    const std::string I2CSensor::I2C_BASE_FOLDER =
        "/sys/class/i2c-adapter/i2c-1/";
    
    std::string I2CSensor::getName()
    {
        return name;
    }
    
    uint16_t I2CSensor::getId()
    {
        return id;
    }
    
    void I2CSensor::doCommand(const std::string& file, std::string& command)
    {
        pc::FileHelper helper(file, O_WRONLY);
        
        helper.writeData(command.c_str(), command.size());
        helper.close();
    }
}
