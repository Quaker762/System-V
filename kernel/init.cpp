/**
 * Kernel initialisation routines
 * 
 * These links should be handy 
 *  http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0438c/CACFEBDD.html
 * 
 */
#include "arch/arm/cpu.h"

#include <kernel/arch/arm/gic.h>
#include <kernel/assertions.h>
#include <kernel/device/keyboard.h>
#include <kernel/device/uart.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/address.h>
#include <kernel/mem/physmem.h>
#include <kernel/mem/virtmem.h>
#include <mjlib/linkedlist.h>

extern "C" void
init()
{
    kprintf("Starting System-V...\n");

    // Relocate the vector table
    kprintf("Relocating vector table to 0x%x...\n", reinterpret_cast<uint32_t>(&vector_table));
    relocate_vector_table();

    kmalloc_init();

    new PhysicalMemoryManager();
    GIC::initialize();
    sti();

    new Keyboard();

    kprintf("entering hang...system halted\n");
    for(;;) {}
}