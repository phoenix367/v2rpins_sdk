/*
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 7, 2013, 7:35 PM
 */

#include <google/protobuf/service.h>
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QMetaType>

#include "MainForm.hpp"
#include "SensorsConnector.hpp"

zmq::context_t gContext(1);
Q_DECLARE_METATYPE(GPSInfo);

int main(int argc, char *argv[]) 
{
    qRegisterMetaType<GPSInfo>();
    
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
