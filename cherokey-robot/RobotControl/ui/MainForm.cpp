/*
 * File:   MainForm.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 19, 2013, 12:37 AM
 */

#include "MainForm.hpp"

MainForm::MainForm() {
    widget.setupUi(this);
    
    connect(widget.btnConnect, SIGNAL(clicked()), SLOT(onConnect()));
    connectorPtr = new RemoteConnector(this);
}

MainForm::~MainForm() 
{
}

void MainForm::onConnect()
{
    connectorPtr->connectToServer("tcp://192.168.1.128:1230");
}
