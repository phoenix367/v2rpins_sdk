/********************************************************************************
** Form generated from reading UI file 'MainForm.ui'
**
** Created: Fri Dec 20 01:07:05 2013
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

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QPushButton *btnConnect;
    QLabel *label;
    QLineEdit *txtConnectionString;
    QFrame *frame;
    QPushButton *btnBackward;
    QPushButton *btnRotateLeft;
    QPushButton *btnForward;
    QPushButton *btnRotateRight;
    QLabel *label_2;

    void setupUi(QDialog *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QString::fromUtf8("MainForm"));
        MainForm->resize(353, 436);
        btnConnect = new QPushButton(MainForm);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(240, 40, 93, 27));
        label = new QLabel(MainForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 131, 17));
        txtConnectionString = new QLineEdit(MainForm);
        txtConnectionString->setObjectName(QString::fromUtf8("txtConnectionString"));
        txtConnectionString->setGeometry(QRect(20, 40, 211, 27));
        frame = new QFrame(MainForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(70, 100, 201, 161));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        btnBackward = new QPushButton(frame);
        btnBackward->setObjectName(QString::fromUtf8("btnBackward"));
        btnBackward->setGeometry(QRect(80, 90, 31, 27));
        btnRotateLeft = new QPushButton(frame);
        btnRotateLeft->setObjectName(QString::fromUtf8("btnRotateLeft"));
        btnRotateLeft->setGeometry(QRect(40, 60, 31, 27));
        btnForward = new QPushButton(frame);
        btnForward->setObjectName(QString::fromUtf8("btnForward"));
        btnForward->setGeometry(QRect(80, 30, 31, 27));
        btnRotateRight = new QPushButton(frame);
        btnRotateRight->setObjectName(QString::fromUtf8("btnRotateRight"));
        btnRotateRight->setGeometry(QRect(120, 60, 31, 27));
        label_2 = new QLabel(MainForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 80, 131, 20));

        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QDialog *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "MainForm", 0, QApplication::UnicodeUTF8));
        btnConnect->setText(QApplication::translate("MainForm", "Connect...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainForm", "Connection string:", 0, QApplication::UnicodeUTF8));
        txtConnectionString->setInputMask(QApplication::translate("MainForm", "*.*.*.*:*; ", 0, QApplication::UnicodeUTF8));
        btnBackward->setText(QString());
        btnRotateLeft->setText(QString());
        btnForward->setText(QString());
        btnRotateRight->setText(QString());
        label_2->setText(QApplication::translate("MainForm", "Movement control", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
