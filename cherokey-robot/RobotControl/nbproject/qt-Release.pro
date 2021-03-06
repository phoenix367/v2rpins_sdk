# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = RobotControl
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui
SOURCES += CommandGrammar.cpp Commands.cpp Globals.cpp GraphPlot.cpp IP4Validator.cpp OdometrySink.cpp PlatformModel.cpp PortValidator.cpp RemoteConnector.cpp SensorsConnector.cpp VideoCaptureFinder.cpp main.cpp messages/common.pb.cc messages/notifications.pb.cc messages/sensors.pb.cc ui/MainForm.cpp ui/ProgramForm.cpp
HEADERS += CommandGrammar.hpp Commands.hpp Globals.hpp GraphPlot.hpp IP4Validator.hpp OdometrySink.hpp PlatformModel.hpp PortValidator.hpp RemoteConnector.hpp SensorsConnector.hpp VideoCaptureFinder.hpp messages/common.pb.h messages/notifications.pb.h messages/sensors.pb.h ui/MainForm.hpp ui/ProgramForm.hpp
FORMS += ui/MainForm.ui ui/ProgramForm.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
