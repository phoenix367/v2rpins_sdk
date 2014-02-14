# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = lib
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = qwtplot3d
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += staticlib debug 
PKGCONFIG +=
QT = core gui opengl
SOURCES += 3rdparty/gl2ps/gl2ps.c src/qwt3d_autoscaler.cpp src/qwt3d_axis.cpp src/qwt3d_color.cpp src/qwt3d_colorlegend.cpp src/qwt3d_coordsys.cpp src/qwt3d_dataviews.cpp src/qwt3d_drawable.cpp src/qwt3d_enrichment_std.cpp src/qwt3d_function.cpp src/qwt3d_gridmapping.cpp src/qwt3d_gridplot.cpp src/qwt3d_io.cpp src/qwt3d_io_gl2ps.cpp src/qwt3d_io_reader.cpp src/qwt3d_label.cpp src/qwt3d_lighting.cpp src/qwt3d_meshplot.cpp src/qwt3d_mousekeyboard.cpp src/qwt3d_movements.cpp src/qwt3d_parametricsurface.cpp src/qwt3d_plot.cpp src/qwt3d_scale.cpp src/qwt3d_surfaceplot.cpp src/qwt3d_types.cpp
HEADERS += 3rdparty/gl2ps/gl2ps.h include/qwt3d_autoptr.h include/qwt3d_autoscaler.h include/qwt3d_axis.h include/qwt3d_color.h include/qwt3d_colorlegend.h include/qwt3d_coordsys.h include/qwt3d_drawable.h include/qwt3d_enrichment.h include/qwt3d_enrichment_std.h include/qwt3d_function.h include/qwt3d_global.h include/qwt3d_graphplot.h include/qwt3d_gridmapping.h include/qwt3d_helper.h include/qwt3d_io.h include/qwt3d_io_gl2ps.h include/qwt3d_io_reader.h include/qwt3d_label.h include/qwt3d_mapping.h include/qwt3d_multiplot.h include/qwt3d_openglhelper.h include/qwt3d_parametricsurface.h include/qwt3d_plot.h include/qwt3d_portability.h include/qwt3d_scale.h include/qwt3d_surfaceplot.h include/qwt3d_types.h include/qwt3d_volumeplot.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = moc
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += include 
LIBS += 
