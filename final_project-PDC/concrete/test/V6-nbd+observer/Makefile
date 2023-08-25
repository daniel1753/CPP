# make clean/all CONFIG=Debug/Release ARCH=linux64/raspi ECHO=0/1

# the parrent dir of "arch" dir
ROOT_DIR = ../../../../..

PROJ_DIR = $(ROOT_DIR)/projects/final_project
CONC_DIR = $(PROJ_DIR)/concrete
FRAME_DIR = $(PROJ_DIR)/framework

#PROJ_SRCS = $(wildcard *.cpp)
PROJ_SRCS = PDC_main.cpp 

# list of directories containing header files
#PROJ_INC_DIR = $(ROOT_DIR)/include
PROJ_INC_DIR = $(CONC_DIR)/include $(FRAME_DIR)/include

# the executable file
PROJ_OUT = PDC_6.out

# list of ADDITIONAL compiler flags (prepended with "-" sign)
#PROJ_CFLAGS = -Wunused-but-set-variable
PROJ_CFLAGS =
PROJ_CPPFLAGS =-std=c++11

#PROJ_MACROS = _BSD_SOURCE
PROJ_MACROS =

# additional object files (created by other projects)
PROJ_ADD_OBJS = select.obj nbd_comm.o ThreadPool.obj Semaphore.obj Dispatcher.obj\
RequestHost.obj Command.obj Transmitter.obj

# additional libraries
PROJ_LIBS =

# linker flags
PROJ_LDFLAGS =

# list of projects (makefiles) this project depends on
PROJ_DEPENDS = $(CONC_DIR)/src/nbd_comm/Makefile $(FRAME_DIR)/src/reactor/Makefile\
$(FRAME_DIR)/src/ThreadPool/Makefile $(FRAME_DIR)/src/Semaphore/Makefile\
$(CONC_DIR)/src/Dispatcher/Makefile $(CONC_DIR)/src/RequestHost/Makefile \
$(CONC_DIR)/src/Command/Makefile $(CONC_DIR)/src/Transmitter/Makefile 

include $(ROOT_DIR)/make/Project.mak 
