/*
 * File:   ProgramForm.cpp
 * Author: Ivan Gubochkin
 *
 * Created on March 23, 2014, 10:39 PM
 */

#include "ProgramForm.hpp"
#include "../CommandGrammar.hpp"

#include <sstream>

ProgramForm::ProgramForm(RemoteConnector *p) 
: connectorPtr(p)
, currentCmdId(-1)
{
    widget.setupUi(this);
    
    connect(widget.btnClose, SIGNAL(clicked()), SLOT(onClose()));
    connect(widget.btnRun, SIGNAL(clicked()), SLOT(onRun()));
}

ProgramForm::~ProgramForm() 
{
}

void ProgramForm::onClose()
{
    hide();
}

void ProgramForm::onRun()
{
    cmdList = std::queue<QSharedPointer<SocketCommand> >();
    
    QString text = widget.textEdit->toPlainText();
    
    std::string str = text.toStdString();
    std::istringstream stream(str);
    
    while (!stream.eof())
    {
        std::string line;
        QSharedPointer<SocketCommand> cmd;

        std::getline(stream, line);
        
        if (!line.empty())
        {
            if (parseCommandStr(line, cmd))
            {
                cmdList.push(cmd);
            }
        }
    }
    
    if (!cmdList.empty())
    {
        start();
    }
}

void ProgramForm::start()
{
    QSharedPointer<SocketCommand> cmd = cmdList.front();
    if (connectorPtr)
    {
        currentCmdId = cmd->getCommandIndex();
        connectorPtr->handleCommand(cmd);
    }
    
    cmdList.pop();
}

void ProgramForm::onCmdResult(quint64 cmdId, bool r)
{
    if (cmdId == currentCmdId && r && !cmdList.empty())
    {
        QSharedPointer<SocketCommand> cmd = cmdList.front();
        if (connectorPtr)
        {
            currentCmdId = cmd->getCommandIndex();
            connectorPtr->handleCommand(cmd);
        }

        cmdList.pop();
    }
}
