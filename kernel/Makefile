AS_OBJS = \
    arch/arm/start.o \
    arch/arm/exception.o

CXX_OBJS = \
    init.o

ASM=arm-none-eabi-as
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld

CXXFLAGS += -std=c++14 -Wall -Wextra -Wold-style-cast
CXXFLAGS += -ffreestanding -mcpu=cortex-a8
CXXFLAGS += -O0 -g3
CXXFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
CXXFLAGS += -MP -MMD
CXXFLAGS += -I./
CXXFLAGS += -I../
CXXFLAGS += -DKERNEL
ASFLAGS += -mcpu=cortex-a8
LDFLAGS += -T linker.ld -nostdlib

DEP = $(CXX_OBJS:%.o=%.d)

ELF_IMAGE=kernel.elf

ELF_IMAGE : $(CXX_OBJS) $(AS_OBJS)
	@echo "LINK $@"; $(LD) $(LDFLAGS) $(CXX_OBJS) $(AS_OBJS) -o $(ELF_IMAGE)

.cpp.o:
	@echo "CXX $@"; $(CXX) $(CXXFLAGS) -o $@ -c $<

.s.o:
	@echo "AS $@"; $(CC) $(ASFLAGS) -o $@ -c $<

clean:
	rm $(CXX_OBJS)
	rm $(AS_OBJS)
	rm $(DEP)
	rm $(ELF_IMAGE)

-include $(DEP)
