/********************************************************************************
** Form generated from reading UI file 'ProgramForm.ui'
**
** Created: Tue Jun 3 23:10:49 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRAMFORM_H
#define UI_PROGRAMFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProgramForm
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTextEdit *txtProgram;
    QLabel *label_2;
    QTextEdit *txtLog;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnRun;
    QPushButton *btnClose;

    void setupUi(QDialog *ProgramForm)
    {
        if (ProgramForm->objectName().isEmpty())
            ProgramForm->setObjectName(QString::fromUtf8("ProgramForm"));
        ProgramForm->resize(645, 518);
        gridLayout_2 = new QGridLayout(ProgramForm);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ProgramForm);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        txtProgram = new QTextEdit(ProgramForm);
        txtProgram->setObjectName(QString::fromUtf8("txtProgram"));

        verticalLayout->addWidget(txtProgram);

        label_2 = new QLabel(ProgramForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        txtLog = new QTextEdit(ProgramForm);
        txtLog->setObjectName(QString::fromUtf8("txtLog"));
        txtLog->setEnabled(false);

        verticalLayout->addWidget(txtLog);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(348, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnRun = new QPushButton(ProgramForm);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));

        horizontalLayout->addWidget(btnRun);

        btnClose = new QPushButton(ProgramForm);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        horizontalLayout->addWidget(btnClose);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(ProgramForm);

        QMetaObject::connectSlotsByName(ProgramForm);
    } // setupUi

    void retranslateUi(QDialog *ProgramForm)
    {
        ProgramForm->setWindowTitle(QApplication::translate("ProgramForm", "ProgramForm", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProgramForm", "Program area", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ProgramForm", "Log output", 0, QApplication::UnicodeUTF8));
        btnRun->setText(QApplication::translate("ProgramForm", "Run", 0, QApplication::UnicodeUTF8));
        btnClose->setText(QApplication::translate("ProgramForm", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProgramForm: public Ui_ProgramForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRAMFORM_H
