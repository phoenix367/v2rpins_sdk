/********************************************************************************
** Form generated from reading UI file 'ProgramForm.ui'
**
** Created: Fri Mar 28 23:42:42 2014
**      by: Qt User Interface Compiler version 4.8.1
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ProgramForm
{
public:
    QTextEdit *textEdit;
    QLabel *label;
    QLabel *label_2;
    QTextEdit *textEdit_2;
    QPushButton *btnRun;
    QPushButton *btnClose;

    void setupUi(QDialog *ProgramForm)
    {
        if (ProgramForm->objectName().isEmpty())
            ProgramForm->setObjectName(QString::fromUtf8("ProgramForm"));
        ProgramForm->resize(645, 518);
        textEdit = new QTextEdit(ProgramForm);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(20, 50, 601, 191));
        label = new QLabel(ProgramForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 101, 17));
        label_2 = new QLabel(ProgramForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 260, 81, 17));
        textEdit_2 = new QTextEdit(ProgramForm);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setEnabled(false);
        textEdit_2->setGeometry(QRect(20, 280, 601, 161));
        btnRun = new QPushButton(ProgramForm);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));
        btnRun->setGeometry(QRect(390, 460, 98, 27));
        btnClose = new QPushButton(ProgramForm);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(520, 460, 98, 27));

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
