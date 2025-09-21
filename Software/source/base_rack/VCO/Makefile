# Project Name
TARGET = VCO

# Sources
CPP_SOURCES = VCO.cpp ../daisy_common.cpp voice.cpp

# Library Locations
LIBDAISY_DIR = ../../../../DaisyExamples/libDaisy/
DAISYSP_DIR = ../../../../DaisyExamples/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

#Flags
LDFLAGS += -u _printf_float