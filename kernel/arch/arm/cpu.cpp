/*
 * Copyright (c) 2020 Jesse Buhagiar <jooster669@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <kernel/arch/arm/cp15.h>
#include <kernel/arch/arm/cpu.h>
#include <kernel/assertions.h>
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
    PREFETCH_ABORT,
    DATA_ABORT,
};

[[noreturn]] static void kpanic(const ExceptionType& type, const RegisterDump& regs)
{
    uint32_t ifsr = CPU::get_ifsr();
    uint32_t dfsr = CPU::get_dfsr();
    uint32_t ifar = CPU::get_ifar();
    uint32_t dfar = CPU::get_dfar();
    uint32_t far = CPU::get_far();

    //TODO: We need to check whether or not this occured in Kernel Mode or User Mode
    kprintf("r0:  0x%x r1: 0x%x r2:   0x%x r3:   0x%x\n", regs.r0, regs.r1, regs.r2, regs.r3);
    kprintf("r4:  0x%x r5: 0x%x r6:   0x%x r7:   0x%x\n", regs.r4, regs.r5, regs.r6, regs.r7);
    kprintf("r8:  0x%x r9: 0x%x r10:  0x%x fp:   0x%x\n", regs.r8, regs.r9, regs.r10, regs.fp);
    kprintf("r12: 0x%x lr: 0x%x sp:   0x%x spsr: 0x%x\n", regs.r12, regs.lr, regs.sp, regs.spsr);
    kprintf("DFSR: 0x%x IFSR: 0x%x\n", dfsr, ifsr);
    kprintf("DFAR: 0x%x IFAR: 0x%x\n", dfar, ifar);
    kprintf("FAR:  0x%x\n", far);
    //kprintf("pid = %d\n", current_proc->pid());
    kprintf("Kernel Panic: ");
    switch(type)
    {
        case ExceptionType::ILLEGAL_INSTRUCTION:
        {
            uint32_t faulting_address = regs.lr - 0x4; // As we are returning to the next instruction, we need to read the previous instruction
            kprintf("Illegal Instruction!\nFaulting address @ 0x%x(0x%x)\n", faulting_address, *reinterpret_cast<uint32_t*>(faulting_address));
            break;
        }
        case ExceptionType::PREFETCH_ABORT:
        {
            uint32_t faulting_address = regs.lr;
            kprintf("Prefetch Abort!\nFaulting address @ 0x%x(0x%x)\n", faulting_address, *reinterpret_cast<uint32_t*>(faulting_address));
            break;
        }
        case ExceptionType::DATA_ABORT:
        {
            uint32_t faulting_address = regs.lr;
            kprintf("Data Abort/Page Fault!\nFaulting address @ 0x%x(0x%x)\n", faulting_address, *reinterpret_cast<uint32_t*>(faulting_address));
            break;
        }
        default:
            break;
    }

    //if(!current_proc)
    //kprintf("current == nullptr!! What the fuck!?\n");

    cli();
    kprintf("System halted\n");
    for(;;)
        ;
}

// This handles the UNDEFINED exception. Illegal Instruction
// is a better term for this imho
extern "C" void illegal_instruction_handler(const RegisterDump& regs)
{
    StatusRegister spsr;

    static_cast<void>(regs);
    if(spsr.get_pre_exception_mode() == ProcessorMode::KERNEL_MODE)
    {
        kprintf("Argh! We crashed in Kernel Mode!\n");
        kpanic(ExceptionType::ILLEGAL_INSTRUCTION, regs);
    }
}

extern "C" void prefetch_abort_handler(const RegisterDump& regs)
{
    StatusRegister spsr;

    static_cast<void>(regs);
    if(spsr.get_pre_exception_mode() == ProcessorMode::KERNEL_MODE)
    {
        kprintf("Argh! We crashed in Kernel Mode!\n");
        kpanic(ExceptionType::PREFETCH_ABORT, regs);
    }
}

extern "C" void data_abort_handler(const RegisterDump& regs)
{
    StatusRegister spsr;

    static_cast<void>(regs);
    if(spsr.get_pre_exception_mode() == ProcessorMode::KERNEL_MODE)
    {
        kprintf("Argh! We crashed in Kernel Mode!\n");
        kpanic(ExceptionType::DATA_ABORT, regs);
    }
}

/*
extern "C" void handle_irq(RegisterDump& regs)
{
    uint16_t irqn = GIC::interrupt_id() & 0xff;

    irq_handlers[irqn]->handle_irq(regs);
    GIC::eoi(irqn);
}
*/
