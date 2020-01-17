/**
 * Kernel initialisation routines
 * 
 * These links should be handy 
 *  http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0438c/CACFEBDD.html
 * 
 */
#include "arch/arm/cpu.h"
#include <kernel/device/uart.h>


extern "C" void init()
{
    relocate_vector_table();
    UART uart0(UART0_BASE);
    uart0.write_string("Hello, World!\n\r\0");
    uart0.write_string("Wow! UART0 works!\n\r\0");
    for(;;){}
}