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

WaitCommand::WaitCommand(uint64_t id, float d)
: IPIDCommand(id)
, duration(d)
{
    
}

WaitCommand::~WaitCommand()
{
    
}

CommandType WaitCommand::getCommandType()
{
    return CommandType::wait;
}

float WaitCommand::getDuration()
{
    return duration;
}

MoveTimeCommand::MoveTimeCommand(uint64_t id, float d, bool r)
: IPIDCommand(id)
, duration(d)
, direction(r)
{
    
}
MoveTimeCommand::~MoveTimeCommand()
{
    
}

CommandType MoveTimeCommand::getCommandType()
{
    return CommandType::moveTime;
}

float MoveTimeCommand::getDuration()
{
    return duration;
}

bool MoveTimeCommand::getDirection()
{
    return direction;
}
