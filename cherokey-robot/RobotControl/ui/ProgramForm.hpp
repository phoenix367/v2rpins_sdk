/* 
 * File:   ProgramForm.hpp
 * Author: Ivan Gubochkin
 *
 * Created on March 23, 2014, 10:39 PM
 */

#ifndef _PROGRAMFORM_HPP
#define	_PROGRAMFORM_HPP

#include "ui_ProgramForm.h"
#include "../CommandGrammar.hpp"
#include "../RemoteConnector.hpp"

#include <queue>

class ProgramForm : public QDialog 
{
    Q_OBJECT
    
public:
    ProgramForm(RemoteConnector *p);
    virtual ~ProgramForm();
    
    Q_SLOT void onClose();
    Q_SLOT void onRun();
    Q_SLOT void onCmdResult(quint64, bool);
    
private:
    void nextCommand();
    
private:
    Ui::ProgramForm widget;
    RemoteConnector *connectorPtr;
    std::queue<QSharedPointer<SocketCommand> > cmdList;
    quint64 currentCmdId;
};

#endif	/* _PROGRAMFORM_HPP */
