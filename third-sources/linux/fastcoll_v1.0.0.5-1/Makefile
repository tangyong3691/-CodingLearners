TOP_DIR ?= .
include $(TOP_DIR)/rules.make
TARGET = $(notdir $(shell pwd))
RELTARGET = $(TARGET)
COMPILE.c = $(CC)  $(C_FLAGS) $(CPP_FLAGS) -I$(THIRDPARTY_DIR)/include   -MD -MP -c
LD_FLAGS += -O2  -o
LD_LIBS += -lstdc++ -lboost_regex -lboost_iostreams -lboost_program_options -lboost_system -lboost_filesystem 
LINK.c = $(CC) $(LD_FLAGS)
RELLDFLAGS += -L./lib -L$(TOP_DIR)/lib $(LD_LIBS)
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
RELOBJFILES = $(SOURCES:%.cpp=%.o)
all: $(RELTARGET)
$(RELTARGET): $(RELOBJFILES)
	$(LINK.c) $@ $^ $(RELLDFLAGS)
$(RELOBJFILES): %.o: %.cpp $(HEADERS)
	$(COMPILE.c) -o $@ $<
clean:
	rm -rf *.o *.d $(TARGET)

