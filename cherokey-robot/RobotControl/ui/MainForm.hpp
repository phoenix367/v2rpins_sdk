/* 
 * File:   MainForm.hpp
 * Author: Ivan Gubochkin
 *
 * Created on December 19, 2013, 12:37 AM
 */

#ifndef _MAINFORM_HPP
#define	_MAINFORM_HPP

#include "ui_MainForm.h"
#include "RemoteConnector.hpp"

class MainForm : public QDialog {
    Q_OBJECT
public:
    MainForm();
    virtual ~MainForm();
    
private:
    Q_SLOT void onConnect();
    
private:
    Ui::MainForm widget;
    RemoteConnector *connectorPtr;
};

#endif	/* _MAINFORM_HPP */
