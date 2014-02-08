#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-none-linux-gnueabi-gcc
CCC=arm-none-linux-gnueabi-g++
CXX=arm-none-linux-gnueabi-g++
FC=gfortran
AS=arm-none-linux-gnueabi-as

# Macros
CND_PLATFORM=V2R-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/messages/common.pb.o \
	${OBJECTDIR}/messages/sensors.pb.o \
	${OBJECTDIR}/src/ConfigManager.o \
	${OBJECTDIR}/src/ConnectionListener.o \
	${OBJECTDIR}/src/DriveController.o \
	${OBJECTDIR}/src/GPSReader.o \
	${OBJECTDIR}/src/IMUReader.o \
	${OBJECTDIR}/src/SensorReader.o \
	${OBJECTDIR}/src/SensorsController.o \
	${OBJECTDIR}/src/VideoController.o \
	${OBJECTDIR}/src/VoltageReader.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/serialstream.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../3pty/lib -lprotobuf -lpthread -lz -lzmq -lboost_program_options -lboost_system -Wl,-rpath,../../pincontroller/dist/Debug/V2R-Linux-x86 -L../../pincontroller/dist/Debug/V2R-Linux-x86 -lpincontroller -lboost_thread -Wl,-rpath,../nmea/dist/Debug/V2R-Linux-x86 -L../nmea/dist/Debug/V2R-Linux-x86 -lnmea -lrt

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver: ../../pincontroller/dist/Debug/V2R-Linux-x86/libpincontroller.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver: ../nmea/dist/Debug/V2R-Linux-x86/libnmea.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/messages/common.pb.o: messages/common.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/messages
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/messages/common.pb.o messages/common.pb.cc

${OBJECTDIR}/messages/sensors.pb.o: messages/sensors.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/messages
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/messages/sensors.pb.o messages/sensors.pb.cc

${OBJECTDIR}/src/ConfigManager.o: src/ConfigManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ConfigManager.o src/ConfigManager.cpp

${OBJECTDIR}/src/ConnectionListener.o: src/ConnectionListener.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ConnectionListener.o src/ConnectionListener.cpp

${OBJECTDIR}/src/DriveController.o: src/DriveController.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DriveController.o src/DriveController.cpp

${OBJECTDIR}/src/GPSReader.o: src/GPSReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/GPSReader.o src/GPSReader.cpp

${OBJECTDIR}/src/IMUReader.o: src/IMUReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/IMUReader.o src/IMUReader.cpp

${OBJECTDIR}/src/SensorReader.o: src/SensorReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SensorReader.o src/SensorReader.cpp

${OBJECTDIR}/src/SensorsController.o: src/SensorsController.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SensorsController.o src/SensorsController.cpp

${OBJECTDIR}/src/VideoController.o: src/VideoController.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VideoController.o src/VideoController.cpp

${OBJECTDIR}/src/VoltageReader.o: src/VoltageReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VoltageReader.o src/VoltageReader.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/serialstream.o: src/serialstream.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -Imessages -I../../pincontroller/include -I../nmea/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/serialstream.o src/serialstream.cpp

# Subprojects
.build-subprojects:
	cd ../../pincontroller && ${MAKE}  -f Makefile CONF=Debug
	cd ../nmea && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver

# Subprojects
.clean-subprojects:
	cd ../../pincontroller && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../nmea && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
