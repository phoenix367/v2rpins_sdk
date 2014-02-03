# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = RobotControl
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui opengl
SOURCES += Commands.cpp IP4Validator.cpp PlatformModel.cpp PortValidator.cpp RemoteConnector.cpp SensorsConnector.cpp main.cpp messages/common.pb.cc messages/sensors.pb.cc ui/MainForm.cpp
HEADERS += Commands.hpp Globals.hpp IP4Validator.hpp PlatformModel.hpp PortValidator.hpp RemoteConnector.hpp SensorsConnector.hpp messages/common.pb.h messages/sensors.pb.h ui/MainForm.hpp
FORMS += ui/MainForm.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = moc
RCC_DIR = 
UI_DIR = ui
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += BOOST_TT_HAS_OPERATOR_HPP_INCLUDED 
INCLUDEPATH += ui messages ../../3pty/include 
LIBS += -lprotobuf -lzmq -lGLU  
CONFIG += link_pkgconfig
PKGCONFIG += QtGStreamerUi-0.10
PKGCONFIG += QtGStreamer-0.10
