#############################################################################
# Makefile for building: dist/Debug/GNU-Linux-x86/RobotControl
# Generated by qmake (2.01a) (Qt 4.8.1) on: Thu Feb 27 01:22:16 2014
# Project:  nbproject/qt-Debug.pro
# Template: app
# Command: /usr/bin/qmake VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED -DQT_SHARED -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB
CFLAGS        = -m64 -pipe -g -pthread -pthread -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -g -pthread -pthread -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -Inbproject -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -Iui -Imessages -I../../3pty/include -I../qwtplot3d/include -I/usr/include/qt4 -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/libxml2 -I/usr/include/QtGStreamer -I/usr/X11R6/include -Imoc -Iui -I.
LINK          = g++
LFLAGS        = -m64
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -L/usr/X11R6/lib64 -lprotobuf -lzmq -lGLU ../qwtplot3d/dist/Debug/GNU-Linux-x86/libqwtplot3d.a -lQtGStreamerUi-0.10 -lQtGStreamer-0.10 -lQtGLib-2.0 -lQtOpenGL -lQtGui -lQtNetwork -lQtCore -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/Debug/GNU-Linux-x86/

####### Files

SOURCES       = Commands.cpp \
		GraphPlot.cpp \
		IP4Validator.cpp \
		PlatformModel.cpp \
		PortValidator.cpp \
		RemoteConnector.cpp \
		SensorsConnector.cpp \
		main.cpp \
		messages/common.pb.cc \
		messages/sensors.pb.cc \
		ui/MainForm.cpp moc/moc_IP4Validator.cpp \
		moc/moc_PlatformModel.cpp \
		moc/moc_PortValidator.cpp \
		moc/moc_RemoteConnector.cpp \
		moc/moc_SensorsConnector.cpp \
		moc/moc_MainForm.cpp
OBJECTS       = build/Debug/GNU-Linux-x86/Commands.o \
		build/Debug/GNU-Linux-x86/GraphPlot.o \
		build/Debug/GNU-Linux-x86/IP4Validator.o \
		build/Debug/GNU-Linux-x86/PlatformModel.o \
		build/Debug/GNU-Linux-x86/PortValidator.o \
		build/Debug/GNU-Linux-x86/RemoteConnector.o \
		build/Debug/GNU-Linux-x86/SensorsConnector.o \
		build/Debug/GNU-Linux-x86/main.o \
		build/Debug/GNU-Linux-x86/common.pb.o \
		build/Debug/GNU-Linux-x86/sensors.pb.o \
		build/Debug/GNU-Linux-x86/MainForm.o \
		build/Debug/GNU-Linux-x86/moc_IP4Validator.o \
		build/Debug/GNU-Linux-x86/moc_PlatformModel.o \
		build/Debug/GNU-Linux-x86/moc_PortValidator.o \
		build/Debug/GNU-Linux-x86/moc_RemoteConnector.o \
		build/Debug/GNU-Linux-x86/moc_SensorsConnector.o \
		build/Debug/GNU-Linux-x86/moc_MainForm.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/link_pkgconfig.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		nbproject/qt-Debug.pro
QMAKE_TARGET  = RobotControl
DESTDIR       = dist/Debug/GNU-Linux-x86/
TARGET        = dist/Debug/GNU-Linux-x86/RobotControl

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: qttmp-Debug.mk $(TARGET)

$(TARGET): ui/ui_MainForm.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) dist/Debug/GNU-Linux-x86/ || $(MKDIR) dist/Debug/GNU-Linux-x86/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9]\+\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

qttmp-Debug.mk: nbproject/qt-Debug.pro  /usr/share/qt4/mkspecs/linux-g++-64/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/link_pkgconfig.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl \
		/usr/lib/x86_64-linux-gnu/libQtOpenGL.prl \
		/usr/lib/x86_64-linux-gnu/libQtNetwork.prl
	$(QMAKE) VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/link_pkgconfig.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
/usr/lib/x86_64-linux-gnu/libQtOpenGL.prl:
/usr/lib/x86_64-linux-gnu/libQtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro

dist: 
	@$(CHK_DIR_EXISTS) build/Debug/GNU-Linux-x86/RobotControl1.0.0 || $(MKDIR) build/Debug/GNU-Linux-x86/RobotControl1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/Debug/GNU-Linux-x86/RobotControl1.0.0/ && $(COPY_FILE) --parents Commands.hpp Globals.hpp GraphPlot.hpp IP4Validator.hpp PlatformModel.hpp PortValidator.hpp RemoteConnector.hpp SensorsConnector.hpp messages/common.pb.h messages/sensors.pb.h ui/MainForm.hpp build/Debug/GNU-Linux-x86/RobotControl1.0.0/ && $(COPY_FILE) --parents Commands.cpp GraphPlot.cpp IP4Validator.cpp PlatformModel.cpp PortValidator.cpp RemoteConnector.cpp SensorsConnector.cpp main.cpp messages/common.pb.cc messages/sensors.pb.cc ui/MainForm.cpp build/Debug/GNU-Linux-x86/RobotControl1.0.0/ && $(COPY_FILE) --parents ui/MainForm.ui build/Debug/GNU-Linux-x86/RobotControl1.0.0/ && (cd `dirname build/Debug/GNU-Linux-x86/RobotControl1.0.0` && $(TAR) RobotControl1.0.0.tar RobotControl1.0.0 && $(COMPRESS) RobotControl1.0.0.tar) && $(MOVE) `dirname build/Debug/GNU-Linux-x86/RobotControl1.0.0`/RobotControl1.0.0.tar.gz . && $(DEL_FILE) -r build/Debug/GNU-Linux-x86/RobotControl1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) qttmp-Debug.mk


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc/moc_IP4Validator.cpp moc/moc_PlatformModel.cpp moc/moc_PortValidator.cpp moc/moc_RemoteConnector.cpp moc/moc_SensorsConnector.cpp moc/moc_MainForm.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc/moc_IP4Validator.cpp moc/moc_PlatformModel.cpp moc/moc_PortValidator.cpp moc/moc_RemoteConnector.cpp moc/moc_SensorsConnector.cpp moc/moc_MainForm.cpp
moc/moc_IP4Validator.cpp: IP4Validator.hpp
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) IP4Validator.hpp -o moc/moc_IP4Validator.cpp

moc/moc_PlatformModel.cpp: PlatformModel.hpp
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) PlatformModel.hpp -o moc/moc_PlatformModel.cpp

moc/moc_PortValidator.cpp: PortValidator.hpp
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) PortValidator.hpp -o moc/moc_PortValidator.cpp

moc/moc_RemoteConnector.cpp: Commands.hpp \
		SensorsConnector.hpp \
		Globals.hpp \
		RemoteConnector.hpp
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) RemoteConnector.hpp -o moc/moc_RemoteConnector.cpp

moc/moc_SensorsConnector.cpp: Globals.hpp \
		SensorsConnector.hpp
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) SensorsConnector.hpp -o moc/moc_SensorsConnector.cpp

moc/moc_MainForm.cpp: ui/ui_MainForm.h \
		PlatformModel.hpp \
		ui/MainForm.hpp
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ui/MainForm.hpp -o moc/moc_MainForm.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: ui/qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) ui/qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui/ui_MainForm.h
compiler_uic_clean:
	-$(DEL_FILE) ui/ui_MainForm.h
ui/ui_MainForm.h: ui/MainForm.ui
	/usr/bin/uic-qt4 ui/MainForm.ui -o ui/ui_MainForm.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

build/Debug/GNU-Linux-x86/Commands.o: Commands.cpp Commands.hpp \
		messages/common.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/Commands.o Commands.cpp

build/Debug/GNU-Linux-x86/GraphPlot.o: GraphPlot.cpp GraphPlot.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/GraphPlot.o GraphPlot.cpp

build/Debug/GNU-Linux-x86/IP4Validator.o: IP4Validator.cpp IP4Validator.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/IP4Validator.o IP4Validator.cpp

build/Debug/GNU-Linux-x86/PlatformModel.o: PlatformModel.cpp PlatformModel.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/PlatformModel.o PlatformModel.cpp

build/Debug/GNU-Linux-x86/PortValidator.o: PortValidator.cpp PortValidator.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/PortValidator.o PortValidator.cpp

build/Debug/GNU-Linux-x86/RemoteConnector.o: RemoteConnector.cpp RemoteConnector.hpp \
		Commands.hpp \
		SensorsConnector.hpp \
		Globals.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/RemoteConnector.o RemoteConnector.cpp

build/Debug/GNU-Linux-x86/SensorsConnector.o: SensorsConnector.cpp SensorsConnector.hpp \
		Globals.hpp \
		messages/sensors.pb.h \
		RemoteConnector.hpp \
		Commands.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/SensorsConnector.o SensorsConnector.cpp

build/Debug/GNU-Linux-x86/main.o: main.cpp SensorsConnector.hpp \
		Globals.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/main.o main.cpp

build/Debug/GNU-Linux-x86/common.pb.o: messages/common.pb.cc messages/common.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/common.pb.o messages/common.pb.cc

build/Debug/GNU-Linux-x86/sensors.pb.o: messages/sensors.pb.cc messages/sensors.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/sensors.pb.o messages/sensors.pb.cc

build/Debug/GNU-Linux-x86/MainForm.o: ui/MainForm.cpp ui/MainForm.hpp \
		ui/ui_MainForm.h \
		PlatformModel.hpp \
		Commands.hpp \
		IP4Validator.hpp \
		PortValidator.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/MainForm.o ui/MainForm.cpp

build/Debug/GNU-Linux-x86/moc_IP4Validator.o: moc/moc_IP4Validator.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_IP4Validator.o moc/moc_IP4Validator.cpp

build/Debug/GNU-Linux-x86/moc_PlatformModel.o: moc/moc_PlatformModel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_PlatformModel.o moc/moc_PlatformModel.cpp

build/Debug/GNU-Linux-x86/moc_PortValidator.o: moc/moc_PortValidator.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_PortValidator.o moc/moc_PortValidator.cpp

build/Debug/GNU-Linux-x86/moc_RemoteConnector.o: moc/moc_RemoteConnector.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_RemoteConnector.o moc/moc_RemoteConnector.cpp

build/Debug/GNU-Linux-x86/moc_SensorsConnector.o: moc/moc_SensorsConnector.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_SensorsConnector.o moc/moc_SensorsConnector.cpp

build/Debug/GNU-Linux-x86/moc_MainForm.o: moc/moc_MainForm.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_MainForm.o moc/moc_MainForm.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

