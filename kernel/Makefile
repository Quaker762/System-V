AS_OBJS = \
    arch/arm/start.o \
    arch/arm/exception.o

CXX_OBJS = \
	arch/abi/cxa_global.o \
	arch/abi/cxa_operator.o \
	arch/abi/cxa_pure.o \
	arch/arm/cpu.o \
	device/uart/uart.o \
	kstdlib/kstring.o \
	kstdlib/kprintf.o \
	memory/heap/blocklist.o \
	memory/heap/heap.o \
    init.o

ASM=arm-none-eabi-as
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld

CXXFLAGS += -std=c++14 -Wall -Wextra -Wold-style-cast
CXXFLAGS += -ffreestanding -march=armv7ve
CXXFLAGS += -O0 -g3 -ggdb
CXXFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
CXXFLAGS += -MP -MMD
CXXFLAGS += -I./
CXXFLAGS += -I../
CXXFLAGS += -DKERNEL
ASFLAGS += -march=armv7ve -g3
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
	rm -f $(CXX_OBJS)
	rm -f $(AS_OBJS)
	rm -f $(DEP)
	rm -f $(ELF_IMAGE)

-include $(DEP)
