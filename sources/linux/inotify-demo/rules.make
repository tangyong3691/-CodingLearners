THIRDPARTY_DIR=./thirdparty
CC = gcc
CFLAGS = -O2
CPP = gcc -E
AR = ar
STRIP = strip
C_FLAGS += -Wall
CPP_FLAGS = -I$(TOP_DIR)/include
LD_FLAGS = -L$(TOP_DIR)/lib
LD_LIBS = 



