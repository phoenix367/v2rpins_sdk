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
#include <QGst/Init>

#include "MainForm.hpp"
#include "SensorsConnector.hpp"

zmq::context_t gContext(1);
Q_DECLARE_METATYPE(GPSInfo);

int main(int argc, char *argv[]) 
{
#if defined(QTVIDEOSINK_PATH)
    //this allows the example to run from the QtGStreamer build tree without installing QtGStreamer
    setenv("GST_PLUGIN_PATH", QTVIDEOSINK_PATH, 0);
#endif
    
    QGst::init();

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
    QGst::cleanup();

    return result;
}
