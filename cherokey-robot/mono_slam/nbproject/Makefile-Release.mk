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
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
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
	${OBJECTDIR}/src/EKF.o \
	${OBJECTDIR}/src/fast.o \
	${OBJECTDIR}/src/fast_9.o \
	${OBJECTDIR}/src/nonmax.o \
	${OBJECTDIR}/src/slam_functions.o \
	${OBJECTDIR}/src/vector_function.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f3 \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmono_slam.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmono_slam.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmono_slam.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmono_slam.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmono_slam.a

${OBJECTDIR}/src/EKF.o: src/EKF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EKF.o src/EKF.cpp

${OBJECTDIR}/src/fast.o: src/fast.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/fast.o src/fast.c

${OBJECTDIR}/src/fast_9.o: src/fast_9.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/fast_9.o src/fast_9.c

${OBJECTDIR}/src/nonmax.o: src/nonmax.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/nonmax.o src/nonmax.c

${OBJECTDIR}/src/slam_functions.o: src/slam_functions.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/slam_functions.o src/slam_functions.cpp

${OBJECTDIR}/src/vector_function.o: src/vector_function.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/vector_function.o src/vector_function.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/ekfTest.o ${TESTDIR}/tests/ekf_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} `pkg-config --libs opencv`   `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/slamFuncTest.o ${TESTDIR}/tests/slamTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} `pkg-config --libs opencv`   `cppunit-config --libs`   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/VfunctionsTest.o ${TESTDIR}/tests/vfunctions_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `pkg-config --libs opencv`   `cppunit-config --libs`   


${TESTDIR}/tests/ekfTest.o: tests/ekfTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/ekfTest.o tests/ekfTest.cpp


${TESTDIR}/tests/ekf_test.o: tests/ekf_test.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/ekf_test.o tests/ekf_test.cpp


${TESTDIR}/tests/slamFuncTest.o: tests/slamFuncTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/slamFuncTest.o tests/slamFuncTest.cpp


${TESTDIR}/tests/slamTest.o: tests/slamTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/slamTest.o tests/slamTest.cpp


${TESTDIR}/tests/VfunctionsTest.o: tests/VfunctionsTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/VfunctionsTest.o tests/VfunctionsTest.cpp


${TESTDIR}/tests/vfunctions_test.o: tests/vfunctions_test.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -std=c++0x `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/vfunctions_test.o tests/vfunctions_test.cpp


${OBJECTDIR}/src/EKF_nomain.o: ${OBJECTDIR}/src/EKF.o src/EKF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/EKF.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -Iinclude -std=c++0x -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EKF_nomain.o src/EKF.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/EKF.o ${OBJECTDIR}/src/EKF_nomain.o;\
	fi

${OBJECTDIR}/src/fast_nomain.o: ${OBJECTDIR}/src/fast.o src/fast.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/fast.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/fast_nomain.o src/fast.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/fast.o ${OBJECTDIR}/src/fast_nomain.o;\
	fi

${OBJECTDIR}/src/fast_9_nomain.o: ${OBJECTDIR}/src/fast_9.o src/fast_9.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/fast_9.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/fast_9_nomain.o src/fast_9.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/fast_9.o ${OBJECTDIR}/src/fast_9_nomain.o;\
	fi

${OBJECTDIR}/src/nonmax_nomain.o: ${OBJECTDIR}/src/nonmax.o src/nonmax.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/nonmax.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/nonmax_nomain.o src/nonmax.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/nonmax.o ${OBJECTDIR}/src/nonmax_nomain.o;\
	fi

${OBJECTDIR}/src/slam_functions_nomain.o: ${OBJECTDIR}/src/slam_functions.o src/slam_functions.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/slam_functions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -Iinclude -std=c++0x -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/slam_functions_nomain.o src/slam_functions.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/slam_functions.o ${OBJECTDIR}/src/slam_functions_nomain.o;\
	fi

${OBJECTDIR}/src/vector_function_nomain.o: ${OBJECTDIR}/src/vector_function.o src/vector_function.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/vector_function.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -O2 -Iinclude -std=c++0x -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/vector_function_nomain.o src/vector_function.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/vector_function.o ${OBJECTDIR}/src/vector_function_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmono_slam.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
