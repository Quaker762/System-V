AS_OBJS = \
	arch/arm/start.o \
    arch/arm/exception.o

CXX_OBJS = \
    arch/abi/cxa_pure.o \
	arch/abi/cxa_global.o \
	arch/abi/cxa_operator.o \
	arch/arm/cpu.o \
	arch/arm/cp15.o \
	arch/arm/gic.o \
	arch/arm/syscall.o \
	kstdlib/kstring.o \
	kstdlib/kprintf.o \
	device/chardevice.o \
	device/device.o \
	device/keyboard.o \
	device/timer.o \
	device/uart.o \
	fs/file.o \
	fs/filedescription.o \
	mem/heap/blocklist.o \
	mem/heap/heap.o \
	mem/vm/l1table.o \
	mem/vm/physpage.o \
	mem/physmem.o \
	irqhandler.o \
	process.o \
	scheduler.o \
	init.o

ASM=arm-none-eabi-as
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld

CXXFLAGS += -std=c++14 -Wall -Wextra -Wold-style-cast
CXXFLAGS += -ffreestanding -march=armv7ve -mcpu=cortex-a15
CXXFLAGS += -O0 -g3
CXXFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
CXXFLAGS += -MP -MMD
CXXFLAGS += -I./
CXXFLAGS += -I../
CXXFLAGS += -DKERNEL
ASFLAGS += -march=armv7ve -mcpu=cortex-a15
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
	rm -rf $(CXX_OBJS)
	rm -rf $(AS_OBJS)
	rm -rf $(DEP)
	rm -rf $(ELF_IMAGE)

-include $(DEP)
