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
#include "../VideoCaptureFinder.hpp"

#include <QMessageBox>
#include <QSharedPointer>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QGst/Parse>
#include <QGridLayout>

#include <iostream>

class Rosenbrock : public Qwt3D::Function
{
public:

    Rosenbrock(Qwt3D::SurfacePlot& pw)
    : Qwt3D::Function(pw)
    {
    }

    double operator()(double x, double y)
    {
        return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
    }
};

MainForm::MainForm()
: connected(false)
, disconnectCmdId(-1)
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
    connect(widget.cmdCommands, SIGNAL(clicked()),
            SLOT(onCommands()));
    
    connectorPtr = new RemoteConnector(this);
    connect(connectorPtr, SIGNAL(ConversationTerminated(const QString&)),
            SLOT(onConnectionTerminated(const QString&)));
    connect(connectorPtr, SIGNAL(CommandSuccess(int, quint64)),
            SLOT(onCommandSuccess(int, quint64)));
    
    programWidget = new ProgramForm(connectorPtr);
    
    widget.frmMove->setEnabled(false);
    widget.btnShowComposite->setEnabled(false);
    
    static IP4Validator ipValidator;
    static PortValidator portValidator;
    
    widget.txtConnectionString->setValidator(&ipValidator);
    widget.txtCommandPort->setValidator(&portValidator);
    widget.txtSensorsPort->setValidator(&portValidator);
    
    qApp->installEventFilter(this);
    
    platformWidget = new PlatformModel(widget.graphicsView);
    QGridLayout *platformGrid = new QGridLayout(widget.graphicsView);
    
    platformGrid->addWidget(platformWidget, 0, 0);

    videoWidget = new QGst::Ui::VideoWidget(widget.tabVideo);
    QGridLayout *videoGrid = new QGridLayout(widget.tabVideo);
    videoGrid->addWidget(videoWidget, 0, 0);
    
    widget.rbtWiFi->setChecked(true);
    
    QGridLayout *grid = new QGridLayout(widget.tabOdometry);
    odometryPlot = new Qwt3D::SurfacePlot(widget.tabOdometry);
    grid->addWidget( odometryPlot, 0, 0 );
    
    Rosenbrock rosenbrock(*odometryPlot);

    rosenbrock.setMesh(41,31);
    rosenbrock.setDomain(-1.73,1.5,-1.5,1.5);
    rosenbrock.setMinZ(-10);

    rosenbrock.create();

    odometryPlot->setRotation(30,0,15);
    odometryPlot->setTitle("Platform IMU odometry");
    odometryPlot->setTitleFont("Arial", 12);
    for (size_t i = 0; i < odometryPlot->coordinates()->axes.size(); ++i)
    {
        odometryPlot->coordinates()->axes[i].setAutoScale(true);
    }

    odometryPlot->coordinates()->setGridLinesColor(Qwt3D::RGBA(0,0,0.5));
    odometryPlot->coordinates()->setLineWidth(1);
    odometryPlot->coordinates()->setNumberFont("Courier",8);
    odometryPlot->coordinates()->adjustNumbers(5);
    
    odometryPlot->coordinates()->axes[Qwt3D::X1].setLabelString("x-axis");
    odometryPlot->coordinates()->axes[Qwt3D::Y1].setLabelString("y-axis");
    odometryPlot->coordinates()->axes[Qwt3D::Z1].setLabelString("z-axis");

    odometryPlot->setCoordinateStyle(Qwt3D::BOX);
    odometryPlot->updateData();
    odometryPlot->updateGL();
    
    odometrySink.enableDrop(true); 
    
    VideoCaptureFinder finder;
}

MainForm::~MainForm() 
{
    connectorPtr->disconnectFromServer();
    stopVideo();
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
        
        connectorPtr->connectToServer(info, programWidget);
        connected = true;
        widget.btnConnect->setText("Disconnect...");
        
        widget.frmMove->setEnabled(true);
        widget.btnShowComposite->setEnabled(true);

        QSharedPointer<SocketCommand> notifyCommand(
            new EnableNotifications(true));
        connectorPtr->handleCommand(notifyCommand);

        QSharedPointer<SocketCommand> sendCommand(
            new SendSensorsInfo(true));
        connectorPtr->handleCommand(sendCommand);
    }
    else
    {
        prepareDisconnect();
    }
}

void MainForm::onConnectionTerminated(const QString& msg)
{
    if (connected)
    {
        doDisconnect();
        QMessageBox::warning(this, "Warning", "Connection error: " + msg);
    }
}

void MainForm::doDisconnect()
{
    connected = false;
    connectorPtr->disconnectFromServer();
    widget.btnConnect->setText("Connect...");
    
    widget.frmMove->setEnabled(false);
    widget.btnShowComposite->setEnabled(false);
    widget.btnShowComposite->setChecked(false);
    
    stopVideo();
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
    ShowVideoComposite::VideoType videoType;
    
    if (widget.rbtRadio->isChecked())
    {
        videoType = ShowVideoComposite::analog;
    }
    else
    {
        videoType = ShowVideoComposite::digital;
    }
    
    if (showVideo)
    {
        startVideo();
    }
    else
    {
        stopVideo();
    }
    
    QSharedPointer<SocketCommand> showCommand(
        new ShowVideoComposite(showVideo, videoType));
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

    if (this->hasFocus())
    {
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

#include <linux/videodev2.h>

void MainForm::startVideo()
{
    QString pipeDescr;

    if (widget.rbtWiFi->isChecked())
    {
        pipeDescr = "udpsrc port=3000 ! application/x-rtp, media=video, "
            "payload=96 ! rtph264depay ! ffdec_h264 ! ffmpegcolorspace ! "
            "videoscale add-borders=true ! "
            "tee name=t ! queue ! appsink name=\"odometry_sink\" t. ! queue ! "
            "ximagesink sync=false";
    }
    else
    {
        VideoCaptureFinder finder;
        std::vector<std::string> devices = finder.getCaptureDevices();
        if (devices.empty())
        {
            return;
        }
        
        std::ostringstream stream;
        stream << "v4l2src device=" << devices[0] << 
                " ! ffmpegcolorspace ! "
                " videoscale add-borders=true ! "
                "tee name=t ! queue ! appsink name=\"odometry_sink\" t. ! queue ! "
                "ximagesink sync=false";
        pipeDescr = QString::fromStdString(stream.str());
    }

    try
    {
        m_pipeline = QGst::Parse::launch(
                pipeDescr).dynamicCast<QGst::Pipeline>();

        if (m_pipeline)
        {
            odometrySink.setElement(m_pipeline->getElementByName("odometry_sink"));

            videoWidget->watchPipeline(m_pipeline);
            m_pipeline->setState(QGst::StatePlaying);
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void MainForm::stopVideo()
{
    if (m_pipeline) 
    {
        videoWidget->stopPipelineWatch();
        m_pipeline->setState(QGst::StateNull);
        m_pipeline.clear();
    }
}

void MainForm::onCommandSuccess(int commandType, quint64 commandIndex)
{
    if (commandType == notificationsCommandType && 
            commandIndex == disconnectCmdId)
    {
        doDisconnect();
    }
}

void MainForm::prepareDisconnect()
{
    QSharedPointer<SocketCommand> showCommand(
        new ShowVideoComposite(false, ShowVideoComposite::VideoType()));
    connectorPtr->handleCommand(showCommand);

    QSharedPointer<SocketCommand> sendCommand(
        new SendSensorsInfo(false));
    connectorPtr->handleCommand(sendCommand);

    QSharedPointer<SocketCommand> notifyCommand(
        new EnableNotifications(false));
    disconnectCmdId = notifyCommand->getCommandIndex();
    connectorPtr->handleCommand(notifyCommand);
}

void MainForm::onCommands()
{
    programWidget->show();
}

void MainForm::onWiFiData(QString interfaceName, float linkQuality, 
        float signalLevel)
{
    if (interfaceName.startsWith("wlan0"))
    {
        widget.lblLinkQuality->setText(QString::number(linkQuality) +
            "/70");
        widget.lblSigLevel->setText(QString::number(signalLevel));
    }
}
