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
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=

# Macros
PLATFORM=GNU-Linux-x86

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Debug/${PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/libocarina.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/${PLATFORM}/liblibocarina.a

dist/Debug/${PLATFORM}/liblibocarina.a: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/${PLATFORM}
	${RM} dist/Debug/${PLATFORM}/liblibocarina.a
	${AR} rv dist/Debug/${PLATFORM}/liblibocarina.a ${OBJECTFILES} 
	$(RANLIB) dist/Debug/${PLATFORM}/liblibocarina.a

${OBJECTDIR}/libocarina.o: libocarina.c 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -g -o ${OBJECTDIR}/libocarina.o libocarina.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/${PLATFORM}/liblibocarina.a

# Subprojects
.clean-subprojects:
