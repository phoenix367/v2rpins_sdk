/*
 * File:   MainForm.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 19, 2013, 12:37 AM
 */

#include "MainForm.hpp"
#include "../Commands.hpp"

#include <QMessageBox>
#include <QSharedPointer>

MainForm::MainForm()
: connected(false)
{
    widget.setupUi(this);
    
    connect(widget.btnConnect, SIGNAL(clicked()), SLOT(onConnect()));
    connect(widget.btnForward, SIGNAL(pressed()), 
            SLOT(onMoveForwardPressed()));
    connect(widget.btnForward, SIGNAL(released()),
            SLOT(onMoveForwardReleased()));
    connect(widget.btnBackward, SIGNAL(pressed()), 
            SLOT(onMoveBackwardPressed()));
    connect(widget.btnBackward, SIGNAL(released()),
            SLOT(onMoveBackwardReleased()));
    connect(widget.btnRotateRight, SIGNAL(pressed()), 
            SLOT(onRotateRightPressed()));
    connect(widget.btnRotateRight, SIGNAL(released()),
            SLOT(onRotateRightReleased()));
    connect(widget.btnRotateLeft, SIGNAL(pressed()), 
            SLOT(onRotateLeftPressed()));
    connect(widget.btnRotateLeft, SIGNAL(released()),
            SLOT(onRotateLeftReleased()));
    connect(widget.btnShowComposite, SIGNAL(clicked()),
            SLOT(onShowVideoComposite()));
    
    connectorPtr = new RemoteConnector(this);
    connect(connectorPtr, SIGNAL(ConversationTerminated(const QString&)),
            SLOT(onConnectionTerminated(const QString&)));
    
    widget.frmMove->setEnabled(false);
    widget.btnShowComposite->setEnabled(false);
}

MainForm::~MainForm() 
{
    connectorPtr->disconnectFromServer();
}

void MainForm::onConnect()
{
    if (!connected)
    {
        QString ipAddress = widget.txtConnectionString->text();
        
        connectorPtr->connectToServer("tcp://" + ipAddress);
        connected = true;
        widget.btnConnect->setText("Disconnect...");
        
        widget.frmMove->setEnabled(true);
        widget.btnShowComposite->setEnabled(true);
    }
    else
    {
        doDisconnect();
    }
}

void MainForm::onConnectionTerminated(const QString& msg)
{
    if (connected)
    {
        doDisconnect();
    }

    QMessageBox::warning(this, "Warning", "Connection error: " + msg);
}

void MainForm::doDisconnect()
{
    connectorPtr->disconnectFromServer();
    connected = false;
    widget.btnConnect->setText("Connect...");
    
    widget.frmMove->setEnabled(false);
    widget.btnShowComposite->setEnabled(false);
}

void MainForm::onMoveForwardPressed()
{
    QSharedPointer<SocketCommand> forwardCommand(new MoveForward(1.0));
    connectorPtr->handleCommand(forwardCommand);
}

void MainForm::onMoveForwardReleased()
{
    QSharedPointer<SocketCommand> forwardCommand(new MoveForward(0.0));
    connectorPtr->handleCommand(forwardCommand);
}

void MainForm::onMoveBackwardPressed()
{
    QSharedPointer<SocketCommand> backwardCommand(new MoveBackward(1.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onMoveBackwardReleased()
{
    QSharedPointer<SocketCommand> backwardCommand(new MoveBackward(0.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onRotateRightPressed()
{
    QSharedPointer<SocketCommand> backwardCommand(new RotateClockwise(1.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onRotateRightReleased()
{
    QSharedPointer<SocketCommand> backwardCommand(new RotateClockwise(0.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onRotateLeftPressed()
{
    QSharedPointer<SocketCommand> backwardCommand(
        new RotateCounterClockwise(1.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onRotateLeftReleased()
{
    QSharedPointer<SocketCommand> backwardCommand(
        new RotateCounterClockwise(0.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onShowVideoComposite()
{
    bool showVideo = widget.btnShowComposite->isChecked();
    
    QSharedPointer<SocketCommand> showCommand(
        new ShowVideoComposite(showVideo));
    connectorPtr->handleCommand(showCommand);
}
