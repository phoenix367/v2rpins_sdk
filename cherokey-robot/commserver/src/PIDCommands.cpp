#include "PIDCommands.hpp"

IPIDCommand::IPIDCommand(uint64_t id)
: commandId(id)
{
    
}

IPIDCommand::~IPIDCommand()
{
    
}

uint64_t IPIDCommand::getCommandId()
{
    return commandId;
}

RotateCommand::RotateCommand(uint64_t id, float angle)
: IPIDCommand(id)
, rotationAngle(angle)
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
