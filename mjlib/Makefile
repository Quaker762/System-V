### GENERAL ###
AS=arm-none-eabi-as
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld
RM = rm -rf

### PROJECT STRUCTURE ###
TARGET = mjlib.a
BLDDIR = build

### PREPROCESSOR FLAGS ###
CPPFLAGS = -I$(INCDIR) -MP -MMD

### COMPILER FLAGS ###
CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -Wold-style-cast
CXXFLAGS += -ffreestanding -march=armv7ve -mcpu=cortex-a15
CXXFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector

### SOURCE, OBJECT AND DEPENDENCY FILES ###
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=$(BLDDIR)/%.o)
DEP = $(SRC:%.cpp=$(BLDDIR)/%.d)

### BUILD RULES ###
.PHONY: all
all: dirs $(TARGET)

.PHONY: dirs
dirs:
	@mkdir -p $(BLDDIR)
	
.PHONY: clean
clean:
	@echo "Deleting library and build files"
	@$(RM) $(BLDDIR)/* $(TARGET)

$(BLDDIR)/%.o: %.cpp
	@echo "Compiling: $< -> $@"
	@$(CC) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

$(TARGET): $(OBJ)
	@echo "Archiving: $@"
	@ar rcs $@ $^ 

-include $(DEP)
