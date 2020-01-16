AS_OBJS = \
	arch/arm/start.o \
    arch/arm/exception.o

CXX_OBJS = \
    arch/abi/cxa_pure.o \
	arch/arm/cpu.o \
	init.o

ASM=arm-none-eabi-as
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld

CXXFLAGS += -std=c++14 -Wall -Wextra -Werror -Wold-style-cast
CXXFLAGS += -ffreestanding -march=armv7ve -mcpu=cortex-a15
CXXFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
CXXFLAGS += -I./
CXXFLAGS += -DKERNEL
ASFLAGS +=
LDFLAGS += -Ttext 0x100000 -T linker.ld -nostdlib

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
	rm $(ELF_IMAGE)

