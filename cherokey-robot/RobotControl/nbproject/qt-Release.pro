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
SOURCES += Commands.cpp RemoteConnector.cpp SensorsListener.cpp main.cpp messages/common.pb.cc messages/sensors.pb.cc ui/MainForm.cpp
HEADERS += Commands.hpp RemoteConnector.hpp SensorsListener.hpp messages/common.pb.h messages/sensors.pb.h ui/MainForm.hpp
FORMS += ui/MainForm.ui
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
