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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/details/dev_helper.o \
	${OBJECTDIR}/src/details/pwm_factory.o \
	${OBJECTDIR}/src/details/pwm_impl.o \
	${OBJECTDIR}/src/pwm.o \
	${OBJECTDIR}/src/servo_rotator.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++0x
CXXFLAGS=-std=c++0x

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpincontroller.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpincontroller.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpincontroller.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -s -fPIC

${OBJECTDIR}/src/details/dev_helper.o: src/details/dev_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/dev_helper.o src/details/dev_helper.cpp

${OBJECTDIR}/src/details/pwm_factory.o: src/details/pwm_factory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/pwm_factory.o src/details/pwm_factory.cpp

${OBJECTDIR}/src/details/pwm_impl.o: src/details/pwm_impl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/pwm_impl.o src/details/pwm_impl.cpp

${OBJECTDIR}/src/pwm.o: src/pwm.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/pwm.o src/pwm.cpp

${OBJECTDIR}/src/servo_rotator.o: src/servo_rotator.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/servo_rotator.o src/servo_rotator.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpincontroller.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
