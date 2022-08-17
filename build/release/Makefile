PROGRAM := vbml

SRC_DIR := ../../src/

CXX := g++
CXFLAGS := -std=c++20 -Wall -pedantic -Werror -g -Og -DNDEBUG -iquote $(SRC_DIR)
LDFLAGS := 

BUILD_DIR := obj/

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
DEPS := $(patsubst %.cpp,%.d,$(patsubst $(SRC_DIR)%,$(BUILD_DIR)%, $(SOURCES)))
OBJS := $(patsubst %.cpp,%.o,$(patsubst $(SRC_DIR)%,$(BUILD_DIR)%, $(SOURCES)))
BUILD_SUBDIRS := $(patsubst $(SRC_DIR)%,$(BUILD_DIR)%, $(shell find $(SRC_DIR) -type d))

all: $(PROGRAM) 

run: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)%.d: $(SRC_DIR)%.cpp | $(BUILD_SUBDIRS)
	$(CXX) $(CXFLAGS) $< -MM -MT '$(patsubst %.cpp,%.o,$(patsubst $(SRC_DIR)%,$(BUILD_DIR)%, $<)) $@' > $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp | $(BUILD_SUBDIRS)
	$(CXX) $(CXFLAGS) -c $< -o $@ 
	
$(BUILD_SUBDIRS):
	mkdir -p $@

demo.html: examples/00-demo.vbml
	./vbml < examples/00-demo.vbml > demo.html

include $(DEPS)

.PHONY: all run clean

clean:
	-rm -rf $(BUILD_DIR) vbml
