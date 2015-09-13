TOP_DIR ?= .
include $(TOP_DIR)/rules.make
TARGET = $(notdir $(shell pwd))
RELTARGET = $(TARGET)
COMPILE.c = $(CC)  $(C_FLAGS) $(CPP_FLAGS) -I$(THIRDPARTY_DIR)/include   -MD -MP -c
LD_FLAGS += -O2  -o 
LINK.c = $(CC) $(LD_FLAGS)
RELLDFLAGS += -L./lib -L$(TOP_DIR)/lib 
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
RELOBJFILES = $(SOURCES:%.c=%.o)
all: $(RELTARGET)
$(RELTARGET): $(RELOBJFILES)
	$(LINK.c) $@ $^ $(RELLDFLAGS)
$(RELOBJFILES): %.o: %.c $(HEADERS)
	$(COMPILE.c) -o $@ $<
clean:
	rm -rf *.o *.d $(TARGET)

