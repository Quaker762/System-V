#include "arch/arm/cpu.h"

#include "assertions.h"

#include <kernel/kstdlib/kstdio.h>

void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func)
{
    kprintf("\nASSERTION FAILED %s\n%s:%d (%s)\n", msg, file, line, func);
    for(;;)
        ;
}

enum class ExceptionType
{
    ILLEGAL_INSTRUCTION,
    DATA_ABORT,
};

[[noreturn]] static void kpanic(const ExceptionType& type, const register_dump& regs)
{
    //TODO: We need to check whether or not this occured in Kernel Mode or User Mode
    kprintf("r0:  0x%x r1: 0x%x r2:   0x%x r3: 0x%x\n", regs.r0, regs.r1, regs.r2, regs.r3);
    kprintf("r4:  0x%x r5: 0x%x r6:   0x%x r7: 0x%x\n", regs.r4, regs.r5, regs.r6, regs.r7);
    kprintf("r8:  0x%x r9: 0x%x r10:  0x%x fp: 0x%x\n", regs.r8, regs.r9, regs.r10, regs.fp);
    kprintf("r12: 0x%x lr: 0x%x spsr: 0x%x\n\n", regs.r12, regs.lr, regs.spsr);
    kprintf("DFSR: 0x%x IFSR: 0x%x\n", 0, 0);
    kprintf("DFAR: 0x%x IFAR: 0x%x\n", 0, 0);
    kprintf("Kernel Panic: ");
    switch(type)
    {
        case ExceptionType::ILLEGAL_INSTRUCTION:
        {
            uint32_t faulting_address = regs.lr - 0x4;
            kprintf("Illegal Instruction!\nFaulting address @ 0x%x(0x%x)\n", faulting_address, *reinterpret_cast<uint32_t*>(faulting_address));
            break;
        }
        case ExceptionType::DATA_ABORT:
        {
            // Read the address that caused the fault
            uint32_t dfar;
            __asm__ volatile("MRC p15, 0, %0, c6, c0, 0"
                             : "=r"(dfar));
            kprintf("Data Abort! Faulting address=0x%x\n", dfar);
            break;
        }
        default:
            break;
    }

    kprintf("System halted\n");
    for(;;)
        ;
}

// This handles the UNDEFINED exception. Illegal Instruction
// is a better term for this imho
extern "C" void illegal_instruction_handler(const register_dump& regs)
{
    StatusRegister spsr;

    static_cast<void>(regs);
    if(spsr.get_pre_exception_mode() == ProcessorMode::KERNEL_MODE)
    {
        kprintf("Argh! We crashed in Kernel Mode!\n");
        kpanic(ExceptionType::ILLEGAL_INSTRUCTION, regs);
    }
}

extern "C" void data_abort_handler(const register_dump& regs)
{
    StatusRegister spsr;

    static_cast<void>(regs);
    if(spsr.get_pre_exception_mode() == ProcessorMode::KERNEL_MODE)
    {
        kprintf("Argh! We crashed in Kernel Mode!\n");
        kpanic(ExceptionType::DATA_ABORT, regs);
    }
}
