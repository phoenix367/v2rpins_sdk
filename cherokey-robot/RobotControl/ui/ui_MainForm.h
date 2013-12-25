/********************************************************************************
** Form generated from reading UI file 'MainForm.ui'
**
** Created: Wed Dec 25 15:50:13 2013
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
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
    QLabel *label_2;
    QFrame *frmSensors;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_3;

    void setupUi(QDialog *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QString::fromUtf8("MainForm"));
        MainForm->resize(566, 296);
        btnConnect = new QPushButton(MainForm);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(240, 40, 93, 27));
        label = new QLabel(MainForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 131, 17));
        txtConnectionString = new QLineEdit(MainForm);
        txtConnectionString->setObjectName(QString::fromUtf8("txtConnectionString"));
        txtConnectionString->setGeometry(QRect(20, 40, 211, 27));
        frmMove = new QFrame(MainForm);
        frmMove->setObjectName(QString::fromUtf8("frmMove"));
        frmMove->setGeometry(QRect(20, 100, 201, 161));
        frmMove->setFrameShape(QFrame::StyledPanel);
        frmMove->setFrameShadow(QFrame::Raised);
        btnBackward = new QToolButton(frmMove);
        btnBackward->setObjectName(QString::fromUtf8("btnBackward"));
        btnBackward->setGeometry(QRect(80, 90, 31, 27));
        btnBackward->setArrowType(Qt::DownArrow);
        btnRotateLeft = new QToolButton(frmMove);
        btnRotateLeft->setObjectName(QString::fromUtf8("btnRotateLeft"));
        btnRotateLeft->setGeometry(QRect(40, 60, 31, 27));
        btnRotateLeft->setArrowType(Qt::LeftArrow);
        btnForward = new QToolButton(frmMove);
        btnForward->setObjectName(QString::fromUtf8("btnForward"));
        btnForward->setGeometry(QRect(80, 30, 31, 27));
        btnForward->setArrowType(Qt::UpArrow);
        btnRotateRight = new QToolButton(frmMove);
        btnRotateRight->setObjectName(QString::fromUtf8("btnRotateRight"));
        btnRotateRight->setGeometry(QRect(120, 60, 31, 27));
        btnRotateRight->setArrowType(Qt::RightArrow);
        label_2 = new QLabel(MainForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 80, 131, 20));
        frmSensors = new QFrame(MainForm);
        frmSensors->setObjectName(QString::fromUtf8("frmSensors"));
        frmSensors->setGeometry(QRect(240, 100, 201, 161));
        frmSensors->setFrameShape(QFrame::StyledPanel);
        frmSensors->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(frmSensors);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 10, 66, 17));
        label_5 = new QLabel(frmSensors);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 30, 66, 17));
        label_3 = new QLabel(MainForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(310, 80, 51, 17));

        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QDialog *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "MainForm", 0, QApplication::UnicodeUTF8));
        btnConnect->setText(QApplication::translate("MainForm", "Connect...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainForm", "Connection string:", 0, QApplication::UnicodeUTF8));
        txtConnectionString->setInputMask(QApplication::translate("MainForm", "000.000.000.000:0000; ", 0, QApplication::UnicodeUTF8));
        btnBackward->setText(QString());
        btnRotateLeft->setText(QString());
        btnForward->setText(QString());
        btnRotateRight->setText(QString());
        label_2->setText(QApplication::translate("MainForm", "Movement control", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainForm", "Voltage:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainForm", "Current:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainForm", "Sensors", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
