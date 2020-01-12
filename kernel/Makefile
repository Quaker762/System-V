AS_OBJS = \
	arch/arm/start.o

CXX_OBJS = \
	init.o

ASM=arm-none-eabi-as
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld

CXXFLAGS += -std=c++14 -Wall -Wextra -Wold-style-cast
CXXFLAGS += -ffreestanding -march=armv7-a -mcpu=cortex-a15
ASFLAGS += -march=armv8-a
LDFLAGS += -Ttext 0x100000 -T linker.ld -nostdlib

ELF_IMAGE=kernel.elf

ELF_IMAGE : $(CXX_OBJS) $(AS_OBJS)
	$(LD) $(LDFLAGS) $(CXX_OBJS) $(AS_OBJS) -o $(ELF_IMAGE)

.cpp.o:
	@echo "CXX $@"; $(CXX) $(CXXFLAGS) -o $@ -c $<

.s.o:
	@echo "AS $@"; $(CC) $(ASFLAGS) -o $@ -c $<


