/**
 * 
 */
#include <kernel/arch/arm/cpu.h>
#include <kernel/kstdlib/kstdio.h>

extern "C" void syscall_handler(RegisterDump& regs)
{
    kprintf("--==SYSCALL==--\n");
    kprintf("r0 0x%x r1 0x%x r2 0x%x r3 0x%x r4 0x%x\n", regs.r0, regs.r1, regs.r2, regs.r3, regs.r4);
}