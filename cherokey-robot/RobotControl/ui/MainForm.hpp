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
    Q_SLOT void onMoveForward();
    Q_SLOT void onMoveBackward();
    Q_SLOT void onRotateLeft();
    Q_SLOT void onRotateRight();
    Q_SLOT void onConnectionTerminated(const QString& msg);
    
private:
    void doDisconnect();
    
private:
    Ui::MainForm widget;
    RemoteConnector *connectorPtr;
    bool connected;
};

#endif	/* _MAINFORM_HPP */
