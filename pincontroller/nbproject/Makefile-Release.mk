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
	${OBJECTDIR}/include/pincontroller/global.o \
	${OBJECTDIR}/src/adc_reader.o \
	${OBJECTDIR}/src/details/dev_helper.o \
	${OBJECTDIR}/src/details/file_helper.o \
	${OBJECTDIR}/src/details/gpio_manager.o \
	${OBJECTDIR}/src/details/gpio_pin_impl.o \
	${OBJECTDIR}/src/details/pwm_factory.o \
	${OBJECTDIR}/src/details/pwm_impl.o \
	${OBJECTDIR}/src/gpio_pin.o \
	${OBJECTDIR}/src/pwm.o \
	${OBJECTDIR}/src/servo_rotator.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

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

${OBJECTDIR}/include/pincontroller/global.o: include/pincontroller/global.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/pincontroller
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/pincontroller/global.o include/pincontroller/global.cpp

${OBJECTDIR}/src/adc_reader.o: src/adc_reader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/adc_reader.o src/adc_reader.cpp

${OBJECTDIR}/src/details/dev_helper.o: src/details/dev_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/dev_helper.o src/details/dev_helper.cpp

${OBJECTDIR}/src/details/file_helper.o: src/details/file_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/file_helper.o src/details/file_helper.cpp

${OBJECTDIR}/src/details/gpio_manager.o: src/details/gpio_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/gpio_manager.o src/details/gpio_manager.cpp

${OBJECTDIR}/src/details/gpio_pin_impl.o: src/details/gpio_pin_impl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/gpio_pin_impl.o src/details/gpio_pin_impl.cpp

${OBJECTDIR}/src/details/pwm_factory.o: src/details/pwm_factory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/pwm_factory.o src/details/pwm_factory.cpp

${OBJECTDIR}/src/details/pwm_impl.o: src/details/pwm_impl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/pwm_impl.o src/details/pwm_impl.cpp

${OBJECTDIR}/src/gpio_pin.o: src/gpio_pin.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/gpio_pin.o src/gpio_pin.cpp

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

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/GpioIndexTest.o ${TESTDIR}/tests/gpionindex.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/tests/GpioIndexTest.o: tests/GpioIndexTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/GpioIndexTest.o tests/GpioIndexTest.cpp


${TESTDIR}/tests/gpionindex.o: tests/gpionindex.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/gpionindex.o tests/gpionindex.cpp


${OBJECTDIR}/include/pincontroller/global_nomain.o: ${OBJECTDIR}/include/pincontroller/global.o include/pincontroller/global.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/pincontroller
	@NMOUTPUT=`${NM} ${OBJECTDIR}/include/pincontroller/global.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/pincontroller/global_nomain.o include/pincontroller/global.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/include/pincontroller/global.o ${OBJECTDIR}/include/pincontroller/global_nomain.o;\
	fi

${OBJECTDIR}/src/adc_reader_nomain.o: ${OBJECTDIR}/src/adc_reader.o src/adc_reader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/adc_reader.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/adc_reader_nomain.o src/adc_reader.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/adc_reader.o ${OBJECTDIR}/src/adc_reader_nomain.o;\
	fi

${OBJECTDIR}/src/details/dev_helper_nomain.o: ${OBJECTDIR}/src/details/dev_helper.o src/details/dev_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/details/dev_helper.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/dev_helper_nomain.o src/details/dev_helper.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/details/dev_helper.o ${OBJECTDIR}/src/details/dev_helper_nomain.o;\
	fi

${OBJECTDIR}/src/details/file_helper_nomain.o: ${OBJECTDIR}/src/details/file_helper.o src/details/file_helper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/details/file_helper.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/file_helper_nomain.o src/details/file_helper.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/details/file_helper.o ${OBJECTDIR}/src/details/file_helper_nomain.o;\
	fi

${OBJECTDIR}/src/details/gpio_manager_nomain.o: ${OBJECTDIR}/src/details/gpio_manager.o src/details/gpio_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/details/gpio_manager.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/gpio_manager_nomain.o src/details/gpio_manager.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/details/gpio_manager.o ${OBJECTDIR}/src/details/gpio_manager_nomain.o;\
	fi

${OBJECTDIR}/src/details/gpio_pin_impl_nomain.o: ${OBJECTDIR}/src/details/gpio_pin_impl.o src/details/gpio_pin_impl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/details/gpio_pin_impl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/gpio_pin_impl_nomain.o src/details/gpio_pin_impl.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/details/gpio_pin_impl.o ${OBJECTDIR}/src/details/gpio_pin_impl_nomain.o;\
	fi

${OBJECTDIR}/src/details/pwm_factory_nomain.o: ${OBJECTDIR}/src/details/pwm_factory.o src/details/pwm_factory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/details/pwm_factory.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/pwm_factory_nomain.o src/details/pwm_factory.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/details/pwm_factory.o ${OBJECTDIR}/src/details/pwm_factory_nomain.o;\
	fi

${OBJECTDIR}/src/details/pwm_impl_nomain.o: ${OBJECTDIR}/src/details/pwm_impl.o src/details/pwm_impl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/details
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/details/pwm_impl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/details/pwm_impl_nomain.o src/details/pwm_impl.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/details/pwm_impl.o ${OBJECTDIR}/src/details/pwm_impl_nomain.o;\
	fi

${OBJECTDIR}/src/gpio_pin_nomain.o: ${OBJECTDIR}/src/gpio_pin.o src/gpio_pin.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/gpio_pin.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/gpio_pin_nomain.o src/gpio_pin.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/gpio_pin.o ${OBJECTDIR}/src/gpio_pin_nomain.o;\
	fi

${OBJECTDIR}/src/pwm_nomain.o: ${OBJECTDIR}/src/pwm.o src/pwm.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/pwm.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/pwm_nomain.o src/pwm.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/pwm_nomain.o;\
	fi

${OBJECTDIR}/src/servo_rotator_nomain.o: ${OBJECTDIR}/src/servo_rotator.o src/servo_rotator.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/servo_rotator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -s -Iinclude -std=c++0x -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/servo_rotator_nomain.o src/servo_rotator.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/servo_rotator.o ${OBJECTDIR}/src/servo_rotator_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpincontroller.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
