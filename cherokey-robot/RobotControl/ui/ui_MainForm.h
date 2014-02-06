/********************************************************************************
** Form generated from reading UI file 'MainForm.ui'
**
** Created: Wed Feb 5 01:33:45 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QPushButton *btnConnect;
    QLabel *label;
    QLineEdit *txtConnectionString;
    QFrame *frmMove;
    QToolButton *btnBackward;
    QToolButton *btnRotateLeft;
    QToolButton *btnForward;
    QToolButton *btnRotateRight;
    QSlider *sldDrivePower;
    QLabel *label_2;
    QFrame *frmSensors;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *lblVoltage;
    QLabel *lblCurrent;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *lblLatitude;
    QLabel *lblLongitude;
    QLabel *label_3;
    QPushButton *btnShowComposite;
    QLabel *label_6;
    QLineEdit *txtCommandPort;
    QLabel *label_7;
    QLineEdit *txtSensorsPort;
    QGraphicsView *graphicsView;
    QLabel *label_11;
    QGraphicsView *videoView;
    QLabel *label_12;
    QFrame *frame;
    QRadioButton *rbtWiFi;
    QRadioButton *rbtRadio;

    void setupUi(QDialog *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QString::fromUtf8("MainForm"));
        MainForm->resize(870, 611);
        btnConnect = new QPushButton(MainForm);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(240, 40, 93, 27));
        label = new QLabel(MainForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 151, 17));
        txtConnectionString = new QLineEdit(MainForm);
        txtConnectionString->setObjectName(QString::fromUtf8("txtConnectionString"));
        txtConnectionString->setGeometry(QRect(20, 40, 211, 27));
        frmMove = new QFrame(MainForm);
        frmMove->setObjectName(QString::fromUtf8("frmMove"));
        frmMove->setGeometry(QRect(20, 330, 201, 161));
        frmMove->setFrameShape(QFrame::StyledPanel);
        frmMove->setFrameShadow(QFrame::Raised);
        btnBackward = new QToolButton(frmMove);
        btnBackward->setObjectName(QString::fromUtf8("btnBackward"));
        btnBackward->setGeometry(QRect(50, 90, 31, 27));
        btnBackward->setArrowType(Qt::DownArrow);
        btnRotateLeft = new QToolButton(frmMove);
        btnRotateLeft->setObjectName(QString::fromUtf8("btnRotateLeft"));
        btnRotateLeft->setGeometry(QRect(10, 60, 31, 27));
        btnRotateLeft->setArrowType(Qt::LeftArrow);
        btnForward = new QToolButton(frmMove);
        btnForward->setObjectName(QString::fromUtf8("btnForward"));
        btnForward->setGeometry(QRect(50, 30, 31, 27));
        btnForward->setArrowType(Qt::UpArrow);
        btnRotateRight = new QToolButton(frmMove);
        btnRotateRight->setObjectName(QString::fromUtf8("btnRotateRight"));
        btnRotateRight->setGeometry(QRect(90, 60, 31, 27));
        btnRotateRight->setArrowType(Qt::RightArrow);
        sldDrivePower = new QSlider(frmMove);
        sldDrivePower->setObjectName(QString::fromUtf8("sldDrivePower"));
        sldDrivePower->setGeometry(QRect(160, 10, 20, 131));
        sldDrivePower->setMaximum(100);
        sldDrivePower->setValue(100);
        sldDrivePower->setOrientation(Qt::Vertical);
        label_2 = new QLabel(MainForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 310, 131, 20));
        frmSensors = new QFrame(MainForm);
        frmSensors->setObjectName(QString::fromUtf8("frmSensors"));
        frmSensors->setGeometry(QRect(240, 330, 201, 161));
        frmSensors->setFrameShape(QFrame::StyledPanel);
        frmSensors->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(frmSensors);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 10, 66, 17));
        label_5 = new QLabel(frmSensors);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 30, 66, 17));
        lblVoltage = new QLabel(frmSensors);
        lblVoltage->setObjectName(QString::fromUtf8("lblVoltage"));
        lblVoltage->setGeometry(QRect(100, 10, 62, 17));
        lblCurrent = new QLabel(frmSensors);
        lblCurrent->setObjectName(QString::fromUtf8("lblCurrent"));
        lblCurrent->setGeometry(QRect(100, 30, 62, 17));
        label_8 = new QLabel(frmSensors);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 60, 31, 17));
        label_9 = new QLabel(frmSensors);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 80, 62, 17));
        label_10 = new QLabel(frmSensors);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 100, 71, 17));
        lblLatitude = new QLabel(frmSensors);
        lblLatitude->setObjectName(QString::fromUtf8("lblLatitude"));
        lblLatitude->setGeometry(QRect(100, 80, 91, 17));
        lblLongitude = new QLabel(frmSensors);
        lblLongitude->setObjectName(QString::fromUtf8("lblLongitude"));
        lblLongitude->setGeometry(QRect(100, 100, 91, 17));
        label_3 = new QLabel(MainForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(310, 310, 51, 17));
        btnShowComposite = new QPushButton(MainForm);
        btnShowComposite->setObjectName(QString::fromUtf8("btnShowComposite"));
        btnShowComposite->setGeometry(QRect(20, 190, 131, 27));
        btnShowComposite->setCheckable(true);
        label_6 = new QLabel(MainForm);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 70, 151, 17));
        txtCommandPort = new QLineEdit(MainForm);
        txtCommandPort->setObjectName(QString::fromUtf8("txtCommandPort"));
        txtCommandPort->setGeometry(QRect(20, 90, 81, 27));
        label_7 = new QLabel(MainForm);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 130, 151, 17));
        txtSensorsPort = new QLineEdit(MainForm);
        txtSensorsPort->setObjectName(QString::fromUtf8("txtSensorsPort"));
        txtSensorsPort->setGeometry(QRect(20, 150, 81, 27));
        graphicsView = new QGraphicsView(MainForm);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(450, 40, 401, 251));
        label_11 = new QLabel(MainForm);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(450, 20, 121, 17));
        videoView = new QGraphicsView(MainForm);
        videoView->setObjectName(QString::fromUtf8("videoView"));
        videoView->setGeometry(QRect(450, 330, 401, 271));
        label_12 = new QLabel(MainForm);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(450, 300, 101, 17));
        frame = new QFrame(MainForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(20, 230, 201, 71));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        rbtWiFi = new QRadioButton(frame);
        rbtWiFi->setObjectName(QString::fromUtf8("rbtWiFi"));
        rbtWiFi->setGeometry(QRect(10, 10, 131, 22));
        rbtRadio = new QRadioButton(frame);
        rbtRadio->setObjectName(QString::fromUtf8("rbtRadio"));
        rbtRadio->setGeometry(QRect(10, 40, 181, 22));

        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QDialog *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "MainForm", 0, QApplication::UnicodeUTF8));
        btnConnect->setText(QApplication::translate("MainForm", "Connect...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainForm", "Connection IP-address:", 0, QApplication::UnicodeUTF8));
        btnBackward->setText(QString());
        btnRotateLeft->setText(QString());
        btnForward->setText(QString());
        btnRotateRight->setText(QString());
        label_2->setText(QApplication::translate("MainForm", "Movement control", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainForm", "Voltage:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainForm", "Current:", 0, QApplication::UnicodeUTF8));
        lblVoltage->setText(QApplication::translate("MainForm", "-", 0, QApplication::UnicodeUTF8));
        lblCurrent->setText(QApplication::translate("MainForm", "-", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainForm", "GPS", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainForm", "Latitude:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainForm", "Longitude:", 0, QApplication::UnicodeUTF8));
        lblLatitude->setText(QApplication::translate("MainForm", "-", 0, QApplication::UnicodeUTF8));
        lblLongitude->setText(QApplication::translate("MainForm", "-", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainForm", "Sensors", 0, QApplication::UnicodeUTF8));
        btnShowComposite->setText(QApplication::translate("MainForm", "Start video", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainForm", "Command server port:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainForm", "Sensors publisher port:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainForm", "Platform position", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainForm", "Platform Video", 0, QApplication::UnicodeUTF8));
        rbtWiFi->setText(QApplication::translate("MainForm", "Video via Wi-Fi", 0, QApplication::UnicodeUTF8));
        rbtRadio->setText(QApplication::translate("MainForm", "Analog Video via RC", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
