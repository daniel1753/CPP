# make clean/all CONFIG=Debug/Release ARCH=linux64/raspi ECHO=0/1

# the parrent dir of "arch" dir
ROOT_DIR = ../../../../..

#PROJ_SRCS = $(wildcard *.cpp)
PROJ_SRCS = Dispatcher.cpp DispatcherTest.cpp

# list of directories containing header files
#PROJ_INC_DIR = $(ROOT_DIR)/include
PROJ_INC_DIR = ../../include ../../../framework/include

# the executable file
PROJ_OUT = Dispatcher.out

# list of ADDITIONAL compiler flags (prepended with "-" sign)
#PROJ_CFLAGS = -Wunused-but-set-variable
PROJ_CFLAGS =
PROJ_CPPFLAGS = -std=c++11

#PROJ_MACROS = _BSD_SOURCE
PROJ_MACROS =

# additional object files (created by other projects)
PROJ_ADD_OBJS = RequestHost.obj  nbd_comm.o  ThreadPool.obj Semaphore.obj Command.obj Transmitter.obj

# additional libraries
PROJ_LIBS =

# linker flags
PROJ_LDFLAGS =

# list of projects (makefiles) this project depends on
PROJ_DEPENDS  =../RequestHost/Makefile ../nbd_comm/Makefile\
../../../framework/src/ThreadPool/Makefile ../../../framework/src/Semaphore/Makefile\
../../../concrete/src/Command/Makefile ../Transmitter/Makefile


include $(ROOT_DIR)/make/Project.mak 
