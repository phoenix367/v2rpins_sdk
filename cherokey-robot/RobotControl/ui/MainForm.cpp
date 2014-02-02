/*
 * File:   MainForm.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 19, 2013, 12:37 AM
 */

#include "MainForm.hpp"
#include "../Commands.hpp"
#include "../IP4Validator.hpp"
#include "../PortValidator.hpp"
#include "../PlatformModel.hpp"

#include <QMessageBox>
#include <QSharedPointer>
#include <QKeyEvent>
#include <QHBoxLayout>

MainForm::MainForm()
: connected(false)
, m_pipeline(NULL)
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
    
    static IP4Validator ipValidator;
    static PortValidator portValidator;
    
    widget.txtConnectionString->setValidator(&ipValidator);
    widget.txtCommandPort->setValidator(&portValidator);
    widget.txtSensorsPort->setValidator(&portValidator);
    
    qApp->installEventFilter(this);
    
    widget.graphicsView->setLayout(new QHBoxLayout());
    platformWidget = new PlatformModel(widget.graphicsView);
    
    QSize widgetSize = widget.graphicsView->size();
    platformWidget->resize(widgetSize.width(), widgetSize.height());
}

MainForm::~MainForm() 
{
    connectorPtr->disconnectFromServer();

    if (m_pipeline) 
    {
        m_pipeline->setState(QGst::StateNull);
    }
}

void MainForm::onConnect()
{
    if (!connected)
    {
        QString ipAddress = widget.txtConnectionString->text();
        
        ConnectionInfo info;
        info.ipAddress = ipAddress;
        info.commandPort = widget.txtCommandPort->text().toInt();
        info.sensorsPort = widget.txtSensorsPort->text().toInt();
        
        connectorPtr->connectToServer(info);
        connected = true;
        widget.btnConnect->setText("Disconnect...");
        
        widget.frmMove->setEnabled(true);
        widget.btnShowComposite->setEnabled(true);

        QSharedPointer<SocketCommand> showCommand(
            new SendSensorsInfo(true));
        connectorPtr->handleCommand(showCommand);
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
    int value = widget.sldDrivePower->value();
    double power = value / 100.0;
    QSharedPointer<SocketCommand> forwardCommand(new MoveForward(power));
    connectorPtr->handleCommand(forwardCommand);
}

void MainForm::onMoveForwardReleased()
{
    QSharedPointer<SocketCommand> forwardCommand(new MoveForward(0.0));
    connectorPtr->handleCommand(forwardCommand);
}

void MainForm::onMoveBackwardPressed()
{
    int value = widget.sldDrivePower->value();
    double power = value / 100.0;
    QSharedPointer<SocketCommand> backwardCommand(new MoveBackward(power));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onMoveBackwardReleased()
{
    QSharedPointer<SocketCommand> backwardCommand(new MoveBackward(0.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onRotateRightPressed()
{
    int value = widget.sldDrivePower->value();
    double power = value / 100.0;
    QSharedPointer<SocketCommand> backwardCommand(new RotateClockwise(power));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onRotateRightReleased()
{
    QSharedPointer<SocketCommand> backwardCommand(new RotateClockwise(0.0));
    connectorPtr->handleCommand(backwardCommand);
}

void MainForm::onRotateLeftPressed()
{
    int value = widget.sldDrivePower->value();
    double power = value / 100.0;
    QSharedPointer<SocketCommand> backwardCommand(
        new RotateCounterClockwise(power));
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

void MainForm::onVoltageData(float voltage, float current)
{
    widget.lblVoltage->setText(QString::number(voltage, 'g', 3));
    widget.lblCurrent->setText(QString::number(current, 'g', 3));
}

bool MainForm::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (!ke->isAutoRepeat())
        {
            switch (ke->key())
            {
                case Qt::Key_W:
                    widget.btnForward->setFocus();
                    onMoveForwardPressed();
                    return true;
                case Qt::Key_S:
                    widget.btnBackward->setFocus();
                    onMoveBackwardPressed();
                    return true;
                case Qt::Key_D:
                    widget.btnRotateRight->setFocus();
                    onRotateRightPressed();
                    return true;
                case Qt::Key_A:
                    widget.btnRotateLeft->setFocus();
                    onRotateLeftPressed();
                    return true;
            }
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (!ke->isAutoRepeat())
        {
            switch (ke->key())
            {
                case Qt::Key_W:
                    widget.btnForward->clearFocus();
                    onMoveForwardReleased();
                    return true;
                case Qt::Key_S:
                    widget.btnBackward->clearFocus();
                    onMoveBackwardReleased();
                    return true;
                case Qt::Key_D:
                    widget.btnRotateRight->clearFocus();
                    onRotateRightReleased();
                    return true;
                case Qt::Key_A:
                    widget.btnRotateLeft->clearFocus();
                    onRotateLeftReleased();
                    return true;
            }
        }
    }
    
    return false;
}

void MainForm::onGPSData(GPSInfo info)
{
    widget.lblLatitude->setText(QString::number(info.latitude, 'g', 10));
    widget.lblLongitude->setText(QString::number(info.longitude, 'g', 10));
}

void MainForm::onModelRotation(float angleX, float angleY, float angleZ)
{
    platformWidget->rotateModel(angleX, angleY, angleZ);
}
