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
#include "../PlatformModel.hpp"
#include "qwt3d_surfaceplot.h"
#include "qwt3d_function.h"
#include "qwt3d_plot.h"

#include <QGst/Pipeline>
#include <QGst/Ui/VideoWidget>

class MainForm : public QDialog 
{
    Q_OBJECT
public:
    MainForm();
    virtual ~MainForm();
    
private:
    Q_SLOT void onConnect();
    Q_SLOT void onConnectionTerminated(const QString& msg);
    Q_SLOT void onVoltageData(float voltage, float current);
    Q_SLOT void onGPSData(GPSInfo info);
    
    Q_SLOT void onMoveForwardPressed();
    Q_SLOT void onMoveForwardReleased();
    Q_SLOT void onMoveBackwardPressed();
    Q_SLOT void onMoveBackwardReleased();
    Q_SLOT void onRotateRightPressed();
    Q_SLOT void onRotateRightReleased();
    Q_SLOT void onRotateLeftPressed();
    Q_SLOT void onRotateLeftReleased();
    Q_SLOT void onShowVideoComposite();
    Q_SLOT void onModelRotation(float angleX, float angleY, float angleZ);
    Q_SLOT void onCommandSuccess(int commandType, uint64_t commandIndex);
    
protected:
    virtual bool eventFilter(QObject *object, QEvent *event);
    
private:
    void doDisconnect();
    void startVideo();
    void stopVideo();
    void prepareDisconnect();
    
private:
    Ui::MainForm widget;
    RemoteConnector *connectorPtr;
    bool connected;
    PlatformModel *platformWidget;
    QGst::PipelinePtr m_pipeline;
    QGst::Ui::VideoWidget *videoWidget;
    Qwt3D::SurfacePlot* odometryPlot;
    uint64_t disconnectCmdId;
};

#endif	/* _MAINFORM_HPP */
