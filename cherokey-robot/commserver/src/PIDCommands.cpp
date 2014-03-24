#include "PIDCommands.hpp"

IPIDCommand::IPIDCommand()
{
    
}

IPIDCommand::~IPIDCommand()
{
    
}

RotateCommand::RotateCommand(float angle)
: rotationAngle(angle)
{
    
}
RotateCommand::~RotateCommand()
{
    
}

CommandType RotateCommand::getCommandType()
{
    return CommandType::rotating;
}

float RotateCommand::getRotationAngle()
{
    return rotationAngle;
}
