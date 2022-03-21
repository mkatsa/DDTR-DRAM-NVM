#=====================================================================================================
#Compilers
CC=gcc
CXX=g++
#=====================================================================================================


#=====================================================================================================
#Inputs and Targets
INPUT=motivational_example.cpp
TARGET=motivation
INPUT2=motivational2_example.cpp
TARGET2=motivation2
#=====================================================================================================


#=====================================================================================================
#Compilation Flags
CXXFLAGS=-g -shared-libgcc -o
DDTRFLAGS=-I./ddtrlibrary
QUARTZFLAGS=-I/NVMemul/quartz/src/lib/ -L/NVMemul/quartz/build/src/lib/ -lnvmemul -DDDTR_QUARTZ
NOLOGGING=-DNOLOGGING
OPTANEFLAGS=-lmemkind -DOPTANE
#=====================================================================================================


#=====================================================================================================
#Variable Section
log="DISABLE"			#Logging is NOT ENABLED by default
emulator="DISABLE"		#Emulator is NOT ENABLED by default
optane="DISABLE"		#Optane is NOT ENABLED by default
#=====================================================================================================


.PHONY: all clean help
.DEFAULT_GOAL := all

help :
	@echo "log 		: ENABLE | DISABLE(DEFAULT)"
	@echo "quartz	: ENABLE | DISABLE(DEFAULT)"
	@echo "optane   : ENABLE | DISABLE(DEFAULT)"

all: motivation motivation2 script 

motivation:
ifeq ($(emulator),ENABLE)
ifeq ($(optane),ENABLE)
	$(error ***************QUARTZ AND OPTANE CANNOT CO-EXIST***************)
endif
endif
ifeq ($(log),ENABLE)
	$(info logging ENABLED)
ifeq ($(emulator),ENABLE)
	$(info emulator ENABLED)
	$(CXX) $(INPUT) $(DDTRFLAGS) $(QUARTZFLAGS) $(CXXFLAGS) $(TARGET)
else
ifeq ($(optane),ENABLE)
	$(info optane ENABLED)
	$(CXX) $(INPUT) $(DDTRFLAGS) $(OPTANEFLAGS) $(CXXFLAGS) $(TARGET)
else
	$(CXX) $(INPUT) $(DDTRFLAGS) $(CXXFLAGS) $(TARGET)
endif
endif
else
ifeq ($(emulator),ENABLE)
	$(info emulator ENABLED)
	$(CXX) $(INPUT) $(DDTRFLAGS) $(NOLOGGING) $(QUARTZFLAGS) $(CXXFLAGS) $(TARGET)
else
ifeq ($(optane),ENABLE)
	$(info optane ENABLED)
	$(CXX) $(INPUT) $(DDTRFLAGS) $(NOLOGGING) $(OPTANEFLAGS) $(CXXFLAGS) $(TARGET)
else
	$(CXX) $(INPUT) $(DDTRFLAGS) $(NOLOGGING) $(CXXFLAGS) $(TARGET)
endif
endif
endif





script: script.o
	$(CXX) script.o -o script

script.o: script.cpp
	$(CXX) -c script.cpp
	
clean:
	rm -f *.o motivation

cleanlog:
	rm -f log_*