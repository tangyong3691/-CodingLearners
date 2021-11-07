TOP_DIR ?= .
include $(TOP_DIR)/rules.make
TARGET = $(notdir $(shell pwd))
RELTARGET = $(TARGET)
COMPILE.c = $(CC) -DHAVE_CONFIG_H $(CFLAGS) $(C_FLAGS) $(CPP_FLAGS) -g -c
LD_FLAGS += -O2  -o 
LINK.c = $(CC) $(LFLAGS) $(LD_FLAGS)
RELLDFLAGS += -L./lib -L$(TOP_DIR)/lib -L$(THIRDLIBDIR) -lrt -lm -lpthread
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
RELOBJFILES = $(SOURCES:%.c=%.o)
all: $(RELTARGET)
$(RELTARGET): $(RELOBJFILES)
	$(LINK.c) $@ $^ $(RELLDFLAGS)
$(RELOBJFILES): %.o: %.c $(HEADERS)
	$(COMPILE.c) -o $@ $<
test:COMPILE.c+= -DTESTPROJECT
test: $(RELOBJFILES)
clean:
	rm -rf *.o *.d $(TARGET)

