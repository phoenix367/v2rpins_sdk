/*
 * File:   MainForm.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 19, 2013, 12:37 AM
 */

#include "MainForm.hpp"
#include <QIcon>
#include <QStyle>

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
}

void MainForm::onConnect()
{
    if (!connected)
    {
        connectorPtr->connectToServer("tcp://192.168.1.128:1230");
        connected = true;
        widget.btnConnect->setText("Disconnect...");
    }
    else
    {
        connectorPtr->disconnectFromServer();
        connected = false;
        widget.btnConnect->setText("Connect...");
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
