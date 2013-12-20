/*
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 7, 2013, 7:35 PM
 */

#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainForm.hpp"
#include "common.pb.h"

int main(int argc, char *argv[]) 
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    MainForm mainForm;
    mainForm.show();

    int result = app.exec();
    google::protobuf::ShutdownProtobufLibrary();

    return result;
}
