# Project Name
TARGET = FHO

# Sources
CPP_SOURCES = FHO.cpp voice.cpp daisy_common.cpp

# Library Locations
LIBDAISY_DIR = ../../../../DaisyExamples/libDaisy/
DAISYSP_DIR = ../../../../DaisyExamples/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float