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
	${OBJECTDIR}/src/ConfigManager.o \
	${OBJECTDIR}/src/main.o


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
LDLIBSOPTIONS=-L../../3pty/lib -lprotobuf -lpthread -lz -lzmq -lboost_program_options

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/messages/common.pb.o: messages/common.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/messages
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/messages/common.pb.o messages/common.pb.cc

${OBJECTDIR}/src/ConfigManager.o: src/ConfigManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ConfigManager.o src/ConfigManager.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -I../../3pty/include -I../../3pty/include/zeromq -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/commserver

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
