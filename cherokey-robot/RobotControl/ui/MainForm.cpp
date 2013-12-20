/*
 * File:   MainForm.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 19, 2013, 12:37 AM
 */

#include "MainForm.hpp"
#include <QIcon>
#include <QStyle>
#include <QRegExpValidator>
#include <QRegExp>
#include <QMessageBox>

MainForm::MainForm()
: connected(false)
{
    widget.setupUi(this);
    
    connect(widget.btnConnect, SIGNAL(clicked()), SLOT(onConnect()));
    connect(widget.btnForward, SIGNAL(clicked()), SLOT(onMoveForward()));
    connect(widget.btnBackward, SIGNAL(clicked()), SLOT(onMoveBackward()));
    connect(widget.btnRotateLeft, SIGNAL(clicked()), SLOT(onRotateLeft()));
    connect(widget.btnRotateRight, SIGNAL(clicked()), SLOT(onRotateRight()));
    
    connectorPtr = new RemoteConnector(this);
    connect(connectorPtr, SIGNAL(ConversationTerminated(const QString&)),
            SLOT(onConnectionTerminated(const QString&)));
    
    QStyle *stylePtr = qApp->style();
    
    QIcon icon = stylePtr->standardIcon(QStyle::SP_ArrowUp);
    widget.btnForward->setIcon(icon);
    
    icon = stylePtr->standardIcon(QStyle::SP_ArrowLeft);
    widget.btnRotateLeft->setIcon(icon);
    
    icon = stylePtr->standardIcon(QStyle::SP_ArrowRight);
    widget.btnRotateRight->setIcon(icon);
    
    icon = stylePtr->standardIcon(QStyle::SP_ArrowDown);
    widget.btnBackward->setIcon(icon);
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
    }
    else
    {
        doDisconnect();
    }
}

void MainForm::onMoveForward()
{
    
}

void MainForm::onMoveBackward()
{
    
}

void MainForm::onRotateLeft()
{
    
}

void MainForm::onRotateRight()
{
    
}

void MainForm::onConnectionTerminated(const QString& msg)
{
    QMessageBox::warning(this, "Warning", "Connection error: " + msg);
    
    if (connected)
    {
        doDisconnect();
    }
}

void MainForm::doDisconnect()
{
    connectorPtr->disconnectFromServer();
    connected = false;
    widget.btnConnect->setText("Connect...");
}
