CC = gcc -g
CFLAGS = -O2
CPP = gcc -E -g
CPLUSPLUS = g++
AR = ar
STRIP = strip
C_FLAGS += -Wall
CPP_FLAGS = -I$(TOP_DIR)/include
LD_FLAGS = -L$(TOP_DIR)/lib

THIRDPARTYDIR=$(TOP_DIR)/third-party
THIRDINCLUDEDIR=$(THIRDPARTYDIR)/include
THIRDLIBDIR=$(THIRDPARTYDIR)/lib/i86
ifeq ($(BITS), 32)
  CFLAGS += -m32
  LFLAGS += -m32
else
ifeq ($(BITS), 64)
  CFLAGS += -m64
  LFLAGS += -m64
  THIRDLIBDIR=$(THIRDPARTYDIR)/lib/x64
endif
endif

