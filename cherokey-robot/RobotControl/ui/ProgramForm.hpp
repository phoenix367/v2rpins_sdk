/* 
 * File:   ProgramForm.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 23, 2014, 10:39 PM
 */

#ifndef _PROGRAMFORM_HPP
#define	_PROGRAMFORM_HPP

#include "ui_ProgramForm.h"

class ProgramForm : public QDialog 
{
    Q_OBJECT
    
public:
    ProgramForm();
    virtual ~ProgramForm();
    
    Q_SLOT void onClose();
    Q_SLOT void onRun();
    
private:
    Ui::ProgramForm widget;
};

#endif	/* _PROGRAMFORM_HPP */
