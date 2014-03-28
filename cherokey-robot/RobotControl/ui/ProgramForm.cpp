/*
 * File:   ProgramForm.cpp
 * Author: Ivan Gubochkin
 *
 * Created on March 23, 2014, 10:39 PM
 */

#include "ProgramForm.hpp"

ProgramForm::ProgramForm() 
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
    
}
