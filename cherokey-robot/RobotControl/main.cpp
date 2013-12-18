/*
 * File:   main.cpp
 * Author: Ivan Gubochkin
 *
 * Created on December 7, 2013, 7:35 PM
 */

#include <QtGui/QApplication>
#include "MainForm.hpp"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    MainForm mainForm;
    mainForm.show();

    return app.exec();
}
